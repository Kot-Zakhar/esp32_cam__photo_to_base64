#include <BtController.h>
#include <EmailController.h>
#include <WiFiController.h>
#include <SDCardController.h>
#include <CameraController.h>
#include <Ticker.h>
#include <IndicationController.h>

#include "logic.h"

#define CONFIG_PIN GPIO_NUM_16
#define SLEEP_TIME 20

// #include "soc/soc.h"           // Disable brownour problems
// #include "soc/rtc_cntl_reg.h"  // Disable brownour problems
bool stateIsConfig;

void processTask() {
  log_d("Connecting to wifi...");
  if (!connectToAnyWiFiFromMemory()) {
    log_d("Couldn't connect.");
    return;
  }

  delay(500);
  
  processCaptureTask();
  
  disconnectFromWiFi();
}

void goToSleep() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, HIGH);
  esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000);
  log_d("Going to sleep. Good night...");
  delay(10);
  esp_deep_sleep_start();
}

void setup() {
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //Disable brownout detector

  Serial.begin(115200);

  initIndication();

  InitMemoryController();

  if (!initCameraController()) {
    log_e("Failed to init Camera.");
    esp_restart();
  }

  pinMode(CONFIG_PIN, INPUT_PULLUP);
  bool isConfigInsisted = !digitalRead(CONFIG_PIN);

  stateIsConfig = isConfigInsisted || IsConfigStateInMemory();

  if (stateIsConfig) {
    log_d("Config mode.");
    indicate(ConfigMode);
    wiFiControllerOff();
    InitBtController();
    return;
  }

  log_d("working mode.");
  indicate(WorkMode);
  processTask();
  
  goToSleep();
}

void loop() {
  // works only when config
  ProcessBt();
}