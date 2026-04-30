#include "drive.h"

static uint32_t usToDuty(int us) {
  return (us / (1000000.0f / SERVO_PWM_FREQ)) * ((1 << SERVO_PWM_RES) - 1);
}

void writeServos(int leftUs, int rightUs) {
  ledcWrite(SERVO_LEFT_PIN, usToDuty(leftUs));
  ledcWrite(SERVO_RIGHT_PIN, usToDuty(rightUs));
}

void initDrive() {
  ledcAttach(SERVO_LEFT_PIN, SERVO_PWM_FREQ, SERVO_PWM_RES);
  ledcAttach(SERVO_RIGHT_PIN, SERVO_PWM_FREQ, SERVO_PWM_RES);
  writeServos(LEFT_NEUTRAL, RIGHT_NEUTRAL);
  // Startup wiggle
  writeServos(LEFT_NEUTRAL + 100, RIGHT_NEUTRAL - 100);
  delay(300);
  writeServos(LEFT_NEUTRAL - 100, RIGHT_NEUTRAL + 100);
  delay(300);
  writeServos(LEFT_NEUTRAL, RIGHT_NEUTRAL);
}

void drive(int throttle, int turn) {
  int left = constrain(throttle + turn, -100, 100);
  int right = constrain(throttle - turn, -100, 100);
  curLeftUs = LEFT_NEUTRAL + map(left, -100, 100, -MAX_OFFSET, MAX_OFFSET);
  curRightUs = RIGHT_NEUTRAL + map(right, -100, 100, MAX_OFFSET, -MAX_OFFSET);
  curThrottle = throttle;
  curTurn = turn;
  pktCount++;
  lastPktTime = millis();
  writeServos(curLeftUs, curRightUs);
}

void stopMotors() {
  curThrottle = 0;
  curTurn = 0;
  writeServos(LEFT_NEUTRAL, RIGHT_NEUTRAL);
}
