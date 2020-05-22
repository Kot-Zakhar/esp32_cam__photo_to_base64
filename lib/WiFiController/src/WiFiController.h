#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#include <WiFi.h>
#include "personal_info_wifi.h"
#include "ArduinoJson.h"
#include "HTTPClient.h"

void connectToWiFi(char *ssid, char *password);
int postBufferToServer(const char *encodedImage, int length, const char *path);

#endif