#include <WiFi.h>

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
