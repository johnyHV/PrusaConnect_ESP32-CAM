#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include <WiFi.h>
#include <ESPmDNS.h>
#include "mcu_cfg.h"

void Wifi_Connect(const char *ssid, const char *password);
void Wifi_Reconnect(const char *ssid, const char *password);

#endif

