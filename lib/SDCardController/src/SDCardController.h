#ifndef _SD_CARD_CONTROLLER_
#define _SD_CARD_CONTROLLER_

#include <FS.h>                // SD Card ESP32
// #include <SD_MMC.h>            // SD Card ESP32
#include <SD.h>

bool initSDCardController();
char *writeToFile(const uint8_t *buf, size_t len, char *fileNameBuf, size_t length, const char* fileExtension);
char *GetDateTimeStr(char *buffer, size_t length);

#endif