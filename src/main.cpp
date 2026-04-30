#include <Arduino.h>
#include "config.h"
#include "drive.h"
#include "camera.h"
#include "network.h"

// ===== SHARED STATE (defined here, declared extern in config.h) =====
volatile int curThrottle = 0, curTurn = 0;
volatile int curLeftUs = LEFT_NEUTRAL, curRightUs = RIGHT_NEUTRAL;
volatile unsigned long pktCount = 0, lastPktTime = 0;
uint8_t activeChannel = DEFAULT_CHANNEL;
bool wifiConnected = false;
String staIP = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n===== LUN-E =====");

  // Set WiFi credentials (remove after first successful connect)
  void setWifiCredentials(const char *, const char *);
  setWifiCredentials("@manjusstudio", "wifi2020!");

  initDrive();
  Serial.println("[DRV] OK");

  if (initCamera()) Serial.println("[CAM] OK");
  else Serial.println("[CAM] FAIL");

  setupNetwork();
  startWebServer();

  Serial.println("[RDY] LUN-E online");
}

void loop() {
  networkLoop();

  // Safety timeout: stop motors if no command for 500ms
  if (lastPktTime > 0 && millis() - lastPktTime > DRIVE_TIMEOUT_MS) {
    if (curThrottle != 0 || curTurn != 0) {
      stopMotors();
    }
  }
}
