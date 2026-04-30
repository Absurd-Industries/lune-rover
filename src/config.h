#pragma once
#include <Arduino.h>

// ===== CAMERA PINS (XIAO ESP32S3 Sense) =====
#define CAM_PIN_PWDN   -1
#define CAM_PIN_RESET  -1
#define CAM_PIN_XCLK   10
#define CAM_PIN_SIOD   40
#define CAM_PIN_SIOC   39
#define CAM_PIN_Y9     48
#define CAM_PIN_Y8     11
#define CAM_PIN_Y7     12
#define CAM_PIN_Y6     14
#define CAM_PIN_Y5     16
#define CAM_PIN_Y4     18
#define CAM_PIN_Y3     17
#define CAM_PIN_Y2     15
#define CAM_PIN_VSYNC  38
#define CAM_PIN_HREF   47
#define CAM_PIN_PCLK   13

// ===== SERVOS =====
#define SERVO_LEFT_PIN   1
#define SERVO_RIGHT_PIN  2
#define SERVO_PWM_FREQ   50
#define SERVO_PWM_RES    12
#define LEFT_NEUTRAL     1500
#define RIGHT_NEUTRAL    1500
#define MAX_OFFSET       200

// ===== NETWORK =====
#define DEFAULT_CHANNEL  6
#define AP_SSID          "LUN-E"
#define AP_PASS          "explorer123"
#define MDNS_NAME        "explorer"

// ===== SAFETY =====
#define DRIVE_TIMEOUT_MS 500

// ===== SHARED STATE =====
typedef struct {
  int8_t throttle;
  int8_t turn;
  uint8_t flags;
} ControlPacket;

extern volatile int curThrottle, curTurn;
extern volatile int curLeftUs, curRightUs;
extern volatile unsigned long pktCount, lastPktTime;
extern uint8_t activeChannel;
extern bool wifiConnected;
extern String staIP;
