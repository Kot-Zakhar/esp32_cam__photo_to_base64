#include <WiFiController.h>

const char* ntpServer1 = "by.pool.ntp.org";
const char* ntpServer2 = "europe.pool.ntp.org";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 0;

void SyncTime(){
  struct tm timeInfo;
  do {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  } while(!getLocalTime(&timeInfo));
  Serial.println(&timeInfo, "Time synced: %A, %b.%d.%Y %H:%M:%S");
}

void connectToWiFi(char *ssid, char *password) {
  WiFi.begin(ssid, password);

  log_i("Connecting to wifi %s", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  log_i("Connected.");

  SyncTime();
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

  http.setTimeout(1000);
  int response = http.POST(buffer);

  free(buffer);

  return response;
}