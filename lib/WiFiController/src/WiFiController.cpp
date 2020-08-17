#include <WiFiController.h>

const char* failedToObtainTimeMessage = "Failed to otain time.";
const char* failedToObtainTimeShortMessage = "<no time>";

const char* timeFormat = "%T";
const char* timeShortFormat = "%R";
const char* dateTimeFormat = "%a %b %d %T";
const char* dateTimeShortFormat = "%T %D";

const char* ntpServer1 = "by.pool.ntp.org";
const char* ntpServer2 = "europe.pool.ntp.org";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 0;

void syncTime(){
  struct tm timeInfo;
  do {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  } while(!getLocalTime(&timeInfo));
  Serial.println(&timeInfo, "Time synced: %A, %b.%d.%Y %H:%M:%S");
}

char *getDateTimeStr(char *buffer, size_t length, bool shortFormat){
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)){
    strcpy(buffer, shortFormat ? failedToObtainTimeShortMessage : failedToObtainTimeMessage);
  } else {
    strftime(buffer, length,
      shortFormat ? dateTimeShortFormat : dateTimeFormat,
      &timeInfo);
  }
  return buffer;
}

void connectToWiFi(char *ssid, char *password) {
  WiFi.begin(ssid, password);

  log_i("Connecting to wifi %s", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  log_i("Connected.");

  syncTime();
}

int postBufferToServer(const char *encodedImage, int length, const char *path) {
  DynamicJsonDocument doc(length + 1000);
  doc["establishmentId"] = "Unknown";
  doc["cameraId"] = "f2b659b3-a10b-42c0-890a-80dcb8df7500";
  
  struct tm timeInfo;
  getLocalTime(&timeInfo);
  time_t seconds = mktime(&timeInfo);
  doc["snapshotTime"] = seconds;
  
  char *nameBuf = (char *)malloc(100);
  strftime(nameBuf, 100, "%c.jpg", &timeInfo);
  doc["name"] = nameBuf;
  free(nameBuf);

  doc["data"] = encodedImage;

  HTTPClient http;
  http.begin(path);

  int lenOfJson = measureJson(doc) + 1;

  char *buffer = (char *)malloc(lenOfJson);

  serializeJson(doc, buffer, lenOfJson);
  http.addHeader("Content-Type", "application/json");

  http.setTimeout(30000);
  int response = http.POST(buffer);

  free(buffer);

  return response;
}



bool isWiFiConnected(){
  return WiFi.status() == WL_CONNECTED;
}

void wiFiControllerOff() {
  disconnectFromWiFi();
}

void disconnectFromWiFi(){
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void awaitForWiFiConnection(){
  log_v("Awaiting for wifi connection...");
  while (!isWiFiConnected())
  {
    log_v(".");
    delay(200);
  }
}

bool awaitForWiFiConnection(int timeout){
  int time = 0;
  log_v("Awaiting for wifi connection...");
  while (!isWiFiConnected() && (time <= timeout))
  {
    log_v(".");
    delay(200);
    time+= 200;
  }
  return isWiFiConnected();
}

void connectToWiFi(const char *ssid, const char *password){
  WiFi.begin(ssid, password);
}

bool connectToAnyWiFiFromMemory(){
  char *ssid = (char *)malloc(STRING_LENGTH);
  char *password = (char *)malloc(STRING_LENGTH);
  int amount = GetWiFiCredentialsAmountFromMemory();
  bool result = false;
  for (int i = 0; (i < amount) && (!result); i++){
    connectToWiFi(GetWiFiSsidFromMemory(i, ssid), GetWiFiPasswordFromMemory(i, password));
    result = awaitForWiFiConnection(2000);
  }
  free(ssid);
  free(password);

  return result;
}