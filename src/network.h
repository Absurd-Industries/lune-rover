#pragma once
#include "config.h"

void setupNetwork();
void networkLoop();
void startWebServer();
void setWifiCredentials(const char *ssid, const char *pass);
