#include <BtController.h>
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

  int bufferLength = 1024;
  char *encodedImage = (char *)malloc(bufferLength);

  encodedImage = takeCaptureToBase64(encodedImage, bufferLength);
  log_d("Current bufferLength: %d", bufferLength);
  int response = postBufferToServer(encodedImage, bufferLength, "http://images-storage-server.herokuapp.com/Image/Send/");
  // int response = postBufferToServer(encodedImage, bufferLength, "http://192.168.0.104:9090/");
  log_d("post response status: %d", response);
  free(encodedImage);
  
  // char *captureName = (char *)malloc(100);
  // // captureName = strcpy(captureName, "TempPic22323-");
  // captureName = takeCaptureAndSaveToSD(captureName, 100);
  // log_i("Took a capture and saved to: %s", captureName);
  // sendLetter("Capture from ESP32-CAM", "Capture is in attachments.", captureName);
  // free(captureName);
}

bool stateIsConfig;

void setup() {

  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //Disable brownout detector


  Serial.begin(115200);

  InitMemoryController();
  if (!initCameraController()) {
    log_e("Failed to init Camera.");
    esp_restart();
  }

  uint8_t resetPin = GPIO_NUM_16;
  pinMode(resetPin, INPUT_PULLUP);
  int resetValue = digitalRead(resetPin);

  stateIsConfig = IsConfigStateInMemory() || !resetValue;

  if (!stateIsConfig) {
    log_d("Connecting to wifi...");
    stateIsConfig = (GetWiFiCredentialsAmountFromMemory() == 0) || !connectToAnyWiFiFromMemory();
    if (stateIsConfig) {
      log_d("Couldn't connect.");
    } else {
      syncTime();
      char *buffer = (char *)malloc(STRING_LENGTH);
      log_d("%s", getDateTimeStr(buffer, STRING_LENGTH));
      free(buffer);
    }
    captureNotifier.attach(60, captureNotify);
    notification = true;
  }

  if (stateIsConfig) {
    log_d("Config mode.");
    wiFiControllerOff();
    InitBtController();
  } else {
    log_d("working mode.");
    // SendLetter(
    //   "First letter",
    //   "I've started to work. Everything is ok so far.",
    //   false
    // );
    initEmailController();
  }



  // if (!initSDCardController()) {
  //   log_e("Failed to init SD card.");
  //   esp_restart();
  // }

  led.attach(1, switchLed);
  pinMode(GPIO_NUM_33, OUTPUT);
}


void loop() {
  if (stateIsConfig){
    ProcessBt();
  } else {
    processCaptureTask();
  }
  delay(100);
}