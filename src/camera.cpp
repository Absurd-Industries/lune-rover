#include "camera.h"

bool initCamera() {
  camera_config_t cfg;
  cfg.ledc_channel = LEDC_CHANNEL_2;
  cfg.ledc_timer = LEDC_TIMER_1;
  cfg.pin_d0 = CAM_PIN_Y2;
  cfg.pin_d1 = CAM_PIN_Y3;
  cfg.pin_d2 = CAM_PIN_Y4;
  cfg.pin_d3 = CAM_PIN_Y5;
  cfg.pin_d4 = CAM_PIN_Y6;
  cfg.pin_d5 = CAM_PIN_Y7;
  cfg.pin_d6 = CAM_PIN_Y8;
  cfg.pin_d7 = CAM_PIN_Y9;
  cfg.pin_xclk = CAM_PIN_XCLK;
  cfg.pin_pclk = CAM_PIN_PCLK;
  cfg.pin_vsync = CAM_PIN_VSYNC;
  cfg.pin_href = CAM_PIN_HREF;
  cfg.pin_sccb_sda = CAM_PIN_SIOD;
  cfg.pin_sccb_scl = CAM_PIN_SIOC;
  cfg.pin_pwdn = CAM_PIN_PWDN;
  cfg.pin_reset = CAM_PIN_RESET;
  cfg.xclk_freq_hz = 20000000;
  cfg.pixel_format = PIXFORMAT_JPEG;
  cfg.grab_mode = CAMERA_GRAB_LATEST;
  cfg.fb_location = CAMERA_FB_IN_PSRAM;
  cfg.jpeg_quality = 8;
  cfg.fb_count = 2;
  cfg.frame_size = FRAMESIZE_QVGA;

  if (esp_camera_init(&cfg) != ESP_OK) return false;

  sensor_t *s = esp_camera_sensor_get();
  if (s) {
    s->set_brightness(s, 1);
    s->set_contrast(s, 1);
    s->set_saturation(s, 0);
    s->set_whitebal(s, 1);
    s->set_awb_gain(s, 1);
    s->set_wb_mode(s, 0);
    s->set_exposure_ctrl(s, 1);
    s->set_aec2(s, 1);
    s->set_gain_ctrl(s, 1);
    s->set_agc_gain(s, 0);
    s->set_bpc(s, 1);
    s->set_wpc(s, 1);
    s->set_lenc(s, 1);
  }
  return true;
}
