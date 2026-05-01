#include "network.h"
#include "drive.h"
#include "camera.h"
#include "page_index.h"
#include "page_wifi.h"
#include "NetworkEvents.h"
#include "NetworkInterface.h"
#include "NetworkManager.h"
#include "ESP32_NOW.h"
#include "WiFi.h"
#include "esp_http_server.h"
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <lwip/sockets.h>

static Preferences prefs;
static DNSServer dnsServer;
static httpd_handle_t webServer = NULL;
static httpd_handle_t streamServer = NULL;

// ==================== ESP-NOW ====================
class ControllerPeer : public ESP_NOW_Peer {
public:
  ControllerPeer(const uint8_t *mac, uint8_t ch)
      : ESP_NOW_Peer(mac, ch, WIFI_IF_STA, nullptr) {}
  bool begin() { return add(); }
  void onReceive(const uint8_t *data, size_t len, bool broadcast) override {
    if (len != sizeof(ControlPacket)) return;
    ControlPacket rx;
    memcpy(&rx, data, sizeof(rx));
    drive(rx.throttle, rx.turn);
  }
};

static std::vector<ControllerPeer *> controllers;

static void onNewPeer(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) != 0) return;
  ControllerPeer *peer = new ControllerPeer(info->src_addr, activeChannel);
  if (!peer->begin()) { delete peer; return; }
  controllers.push_back(peer);
  Serial.printf("[NOW] Paired CH%d\n", activeChannel);
}

// ==================== HTTP HANDLERS ====================
static const char *STREAM_CT = "multipart/x-mixed-replace;boundary=frame";
static const char *STREAM_BD = "\r\n--frame\r\n";
static const char *STREAM_PT = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static esp_err_t streamHandler(httpd_req_t *req) {
  camera_fb_t *fb = NULL;
  char part[64];
  httpd_resp_set_type(req, STREAM_CT);
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) { httpd_resp_send_500(req); return ESP_FAIL; }
    size_t hl = snprintf(part, sizeof(part), STREAM_PT, fb->len);
    esp_err_t r = httpd_resp_send_chunk(req, STREAM_BD, strlen(STREAM_BD));
    if (r == ESP_OK) r = httpd_resp_send_chunk(req, part, hl);
    if (r == ESP_OK) r = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
    if (r != ESP_OK) break;
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  return ESP_OK;
}

static esp_err_t driveHandler(httpd_req_t *req) {
  char buf[64];
  int len = httpd_req_get_url_query_len(req);
  if (len > 0 && len < (int)sizeof(buf)) {
    httpd_req_get_url_query_str(req, buf, sizeof(buf));
    char tVal[8] = "0", sVal[8] = "0";
    httpd_query_key_value(buf, "t", tVal, sizeof(tVal));
    httpd_query_key_value(buf, "s", sVal, sizeof(sVal));
    drive(atoi(tVal), atoi(sVal));
  }
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_sendstr(req, "ok");
}

// ==================== WEBSOCKET ====================
static esp_err_t wsHandler(httpd_req_t *req) {
  if (req->method == HTTP_GET) return ESP_OK;
  httpd_ws_frame_t frame;
  uint8_t buf[32];
  memset(&frame, 0, sizeof(frame));
  frame.payload = buf;
  frame.type = HTTPD_WS_TYPE_TEXT;
  esp_err_t ret = httpd_ws_recv_frame(req, &frame, sizeof(buf) - 1);
  if (ret != ESP_OK) return ret;
  buf[frame.len] = 0;
  int t = 0, s = 0;
  char *comma = strchr((char *)buf, ',');
  if (comma) { *comma = 0; t = atoi((char *)buf); s = atoi(comma + 1); }
  drive(t, s);
  return ESP_OK;
}

static esp_err_t statusHandler(httpd_req_t *req) {
  char buf[384];
  snprintf(buf, sizeof(buf),
    "{\"thr\":%d,\"trn\":%d,\"lus\":%d,\"rus\":%d,\"pkt\":%lu,\"up\":%lu,"
    "\"peers\":%d,\"clients\":%d,\"ch\":%d,\"wifi\":%s,\"ip\":\"%s\"}",
    (int)curThrottle, (int)curTurn, (int)curLeftUs, (int)curRightUs,
    pktCount, millis() / 1000, (int)controllers.size(),
    WiFi.softAPgetStationNum(), activeChannel,
    wifiConnected ? "true" : "false", staIP.c_str());
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, buf, strlen(buf));
}

static esp_err_t scanHandler(httpd_req_t *req) {
  WiFi.scanDelete();
  int n = WiFi.scanNetworks(false, false, false, 1000);
  String json = "[";
  for (int i = 0; i < n; i++) {
    if (WiFi.SSID(i).length() == 0) continue;
    if (json.length() > 1) json += ",";
    String ssid = WiFi.SSID(i);
    ssid.replace("\"", "\\\"");
    json += "{\"ssid\":\"" + ssid + "\",\"rssi\":" + String(WiFi.RSSI(i)) + ",\"ch\":" + String(WiFi.channel(i)) + "}";
  }
  json += "]";
  WiFi.scanDelete();
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json.c_str(), json.length());
}

static esp_err_t indexHandler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, PAGE_INDEX, strlen(PAGE_INDEX));
}

static esp_err_t wifiPageHandler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, PAGE_WIFI, strlen(PAGE_WIFI));
}

static esp_err_t wifiSaveHandler(httpd_req_t *req) {
  char body[256];
  int len = httpd_req_recv(req, body, sizeof(body) - 1);
  if (len <= 0) { httpd_resp_send_500(req); return ESP_FAIL; }
  body[len] = 0;

  String bodyStr = String(body);
  String ssid = "", pass = "";
  int si = bodyStr.indexOf("ssid=");
  int pi = bodyStr.indexOf("pass=");
  if (si >= 0) { int e = bodyStr.indexOf('&', si); ssid = bodyStr.substring(si + 5, e > 0 ? e : bodyStr.length()); }
  if (pi >= 0) { int e = bodyStr.indexOf('&', pi); pass = bodyStr.substring(pi + 5, e > 0 ? e : bodyStr.length()); }

  ssid.replace("+", " ");
  pass.replace("+", " ");
  for (int c = 32; c < 128; c++) {
    char upper[4], lower[4], ch[2] = {(char)c, 0};
    snprintf(upper, sizeof(upper), "%%%02X", c);
    snprintf(lower, sizeof(lower), "%%%02x", c);
    ssid.replace(upper, ch); ssid.replace(lower, ch);
    pass.replace(upper, ch); pass.replace(lower, ch);
  }

  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();

  httpd_resp_set_type(req, "text/html");
  httpd_resp_send(req, PAGE_WIFI_SAVED, strlen(PAGE_WIFI_SAVED));
  delay(1500);
  ESP.restart();
  return ESP_OK;
}

static esp_err_t wifiResetHandler(httpd_req_t *req) {
  prefs.begin("wifi", false); prefs.clear(); prefs.end();
  httpd_resp_sendstr(req, "RESET. REBOOTING...");
  delay(1000);
  ESP.restart();
  return ESP_OK;
}

static esp_err_t captiveRedirect(httpd_req_t *req) {
  httpd_resp_set_status(req, "302 Found");
  httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/wifi");
  return httpd_resp_send(req, NULL, 0);
}

// ==================== WEB SERVER ====================
void startWebServer() {
  // Stream server on port 81 — pinned to core 0
  httpd_config_t streamCfg = HTTPD_DEFAULT_CONFIG();
  streamCfg.server_port = 81;
  streamCfg.ctrl_port = 32769;
  streamCfg.stack_size = 12288;
  streamCfg.max_uri_handlers = 2;
  streamCfg.core_id = 0;

  if (httpd_start(&streamServer, &streamCfg) == ESP_OK) {
    httpd_uri_t streamRoute = { "/stream", HTTP_GET, streamHandler, NULL };
    httpd_register_uri_handler(streamServer, &streamRoute);
    Serial.println("[WEB] Stream on :81 core0");
  }

  // Control server on port 80 — pinned to core 1, high priority, low-latency
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  config.stack_size = 8192;
  config.max_uri_handlers = 16;
  config.core_id = 1;
  config.task_priority = configMAX_PRIORITIES - 1;
  config.recv_wait_timeout = 1;
  config.send_wait_timeout = 1;
  config.open_fn = [](httpd_handle_t hd, int sockfd) -> esp_err_t {
    int nodelay = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    return ESP_OK;
  };

  if (httpd_start(&webServer, &config) != ESP_OK) return;

  httpd_uri_t wsRoute = { "/ws", HTTP_GET, wsHandler, NULL, .is_websocket = true };
  httpd_register_uri_handler(webServer, &wsRoute);

  httpd_uri_t routes[] = {
    { "/",                HTTP_GET,  indexHandler,     NULL },
    { "/drive",           HTTP_GET,  driveHandler,     NULL },
    { "/status",          HTTP_GET,  statusHandler,    NULL },
    { "/wifi",            HTTP_GET,  wifiPageHandler,  NULL },
    { "/wifi-save",       HTTP_POST, wifiSaveHandler,  NULL },
    { "/wifi-reset",      HTTP_GET,  wifiResetHandler, NULL },
    { "/scan",            HTTP_GET,  scanHandler,      NULL },
    { "/generate_204",    HTTP_GET,  captiveRedirect,  NULL },
    { "/hotspot-detect.html", HTTP_GET, captiveRedirect, NULL },
    { "/connecttest.txt", HTTP_GET,  captiveRedirect,  NULL },
    { "/redirect",        HTTP_GET,  captiveRedirect,  NULL },
    { "/success.txt",     HTTP_GET,  captiveRedirect,  NULL },
  };
  for (auto &r : routes) httpd_register_uri_handler(webServer, &r);

  httpd_register_err_handler(webServer, HTTPD_404_NOT_FOUND,
    [](httpd_req_t *req, httpd_err_code_t err) -> esp_err_t {
      httpd_resp_set_status(req, "302 Found");
      httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/wifi");
      return httpd_resp_send(req, NULL, 0);
    });

  Serial.println("[WEB] Server on :80");
}

// ==================== NETWORK SETUP ====================
void setupNetwork() {
  prefs.begin("wifi", true);
  String ssid = prefs.getString("ssid", "");
  String pass = prefs.getString("pass", "");
  prefs.end();

  WiFi.setSleep(false);

  if (ssid.length() > 0) {
    Serial.printf("[NET] Connecting '%s'...\n", ssid.c_str());
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASS);
    delay(100);
    WiFi.begin(ssid.c_str(), pass.c_str());

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 30) { delay(500); Serial.print("."); tries++; }

    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
      staIP = WiFi.localIP().toString();
      activeChannel = WiFi.channel();
      WiFi.softAP(AP_SSID, AP_PASS, activeChannel);
      Serial.printf("\n[NET] IP=%s CH=%d\n", staIP.c_str(), activeChannel);
    } else {
      Serial.printf("\n[NET] Failed, AP-only\n");
      WiFi.disconnect();
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(AP_SSID, AP_PASS, DEFAULT_CHANNEL);
      activeChannel = DEFAULT_CHANNEL;
    }
  } else {
    Serial.println("[NET] No saved WiFi, AP-only");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASS, DEFAULT_CHANNEL);
    activeChannel = DEFAULT_CHANNEL;
  }

  if (!wifiConnected) {
    dnsServer.start(53, "*", WiFi.softAPIP());
    Serial.printf("[NET] Captive portal -> %s\n", WiFi.softAPIP().toString().c_str());
  }

  delay(100);

  if (MDNS.begin(MDNS_NAME)) {
    MDNS.addService("http", "tcp", 80);
    Serial.printf("[NET] %s.local\n", MDNS_NAME);
  }

  if (!ESP_NOW.begin()) ESP.restart();
  ESP_NOW.onNewPeer(onNewPeer, nullptr);
  Serial.printf("[NET] MAC=%s CH=%d\n", WiFi.macAddress().c_str(), activeChannel);
}

void networkLoop() {
  if (!wifiConnected) dnsServer.processNextRequest();
}

void setWifiCredentials(const char *ssid, const char *pass) {
  prefs.begin("wifi", false);
  prefs.clear();
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
}
