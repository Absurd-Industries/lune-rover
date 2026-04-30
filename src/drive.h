#pragma once
#include "config.h"

void initDrive();
void drive(int throttle, int turn);
void stopMotors();
void writeServos(int leftUs, int rightUs);
