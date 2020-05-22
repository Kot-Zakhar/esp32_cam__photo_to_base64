#ifndef _WIFI_CONTROLLER_H_
#define _WIFI_CONTROLLER_H_

#include <WiFi.h>
#include <MemoryController.h>
#include "personal_info_wifi.h"
#include "ArduinoJson.h"
#include "HTTPClient.h"

void initWiFiController();
void wiFiControllerOff();
void syncTime();

void connectToWiFi(char *ssid, char *password);
int postBufferToServer(const char *encodedImage, int length, const char *path);

bool isWiFiConnected();
bool connectToAnyWiFiFromMemory();
void disconnectFromWiFi();
char *getDateTimeStr(char *buffer, size_t length, bool shortFormat = true);

#endif