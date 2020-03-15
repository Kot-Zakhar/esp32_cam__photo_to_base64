#include <Arduino.h>
#include <EmailController.h>
#include <WiFiController.h>
#include <SDCardController.h>
#include <CameraController.h>
#include <Ticker.h>

// #include "soc/soc.h"           // Disable brownour problems
// #include "soc/rtc_cntl_reg.h"  // Disable brownour proble
Ticker led;
Ticker captureNotifier;
bool notification = false;
bool ledState = false;

void switchLed() {
  ledState = !ledState;
  digitalWrite(GPIO_NUM_33, ledState);
}

void IRAM_ATTR captureNotify() {
  notification = true;
}

void processCaptureTask() {
  if (!notification)
    return;
  notification = false;

  char *captureName = (char *)malloc(100);
  // captureName = strcpy(captureName, "TempPic22323-");
  captureName = takeCaptureAndSaveToSD(captureName, 100);
  log_i("Took a capture and saved to: %s", captureName);
  sendLetter("Capture from ESP32-CAM", "Capture is in attachments.", captureName);
  free(captureName);
}


void setup() {

  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //Disable brownout detector

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Started");

  connectToWiFi(PERSONAL_WIFI_SSID, PERSONAL_WIRI_PASSWORD);

  if (!initSDCardController()) {
    log_e("Failed to init SD card.");
    esp_restart();
  }

  if (!initCameraController()) {
    log_e("Failed to init Camera.");
    esp_restart();
  }

  delay(500);

  captureNotifier.attach(60, captureNotify);
  notification = true;
  led.attach(1, switchLed);
  pinMode(GPIO_NUM_33, OUTPUT);
}

void loop() {
  processCaptureTask();
}
