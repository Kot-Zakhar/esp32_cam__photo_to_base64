#include <WiFiController.h>
#include <CameraController.h>
#include "logic.h"


void processCaptureTask() {
  int bufferLength = 1024;
  char *encodedImage = (char *)malloc(bufferLength);

  encodedImage = takeCaptureToBase64(encodedImage, bufferLength);
  log_d("Current bufferLength: %d", bufferLength);
  int response = postBufferToServer(encodedImage, bufferLength, "http://images-storage-server.herokuapp.com/Image/Send/");
  // int response = postBufferToServer(encodedImage, bufferLength, "http://192.168.0.104:9090/");
  log_d("post response status: %d", response);
  free(encodedImage);
}