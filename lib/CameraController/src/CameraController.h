#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include <SDCardController.h>
#include <esp_camera.h>
#include <img_converters.h>

bool initCameraController();
void turnLightOff();
void turnLightOn();
char *takeCaptureAndSaveToSD(char *fileNameBuf, size_t fileNameLen);
char *takeCaptureToBase64(char *encodedBuffer, int &bufferLength);

#endif