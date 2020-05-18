#include "SDCardController.h"

#define FILENAME_LENGTH 100

bool initSDCardController() {
	SPI.begin(14, 2, 15, 13);
	if(!SD.begin(13, SPI, 4000000U, "/sdcard")) {
  // if(!SD_MMC.begin("/sdcard", true)){
    log_e("SD Card Mount Failed");
    return false;
  }
  
	uint8_t cardType = SD.cardType();
  // uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    log_e("No SD Card attached");
    return false;
  }

  return true;
}

// if fileNameBuf is empty, date+time is used by default
// fileNameBuf can't be null
char *writeToFile(const uint8_t *buf, size_t len, char *fileNameBuf, size_t length, const char* fileExtension) {
	if (fileNameBuf == NULL) {
		log_e("Null pointer was provided");
		return NULL;
	}


	char *fileName = (char *)malloc(FILENAME_LENGTH);
	fileName = strcpy(fileName, "/");
	
	if (!strnlen(fileNameBuf, length)) {
		fileNameBuf = GetDateTimeStr(fileNameBuf, length);
	}
	fileName = strcat(fileName, fileNameBuf);
	fileName = strcat(fileName, fileExtension);

	fileNameBuf = strncpy(fileNameBuf, fileName, length);
	free(fileName);


	// log_i("File will be saved to: %s", fileName);

	// FS &fs = SD_MMC;
	FS &fs = SD;

	File file = fs.open(fileNameBuf, FILE_WRITE);
	if (!file) {
		log_e("File %s can't be opened for write.", fileNameBuf);
	} else {
		file.write(buf, len);
		file.close();
	}

	return fileNameBuf;
}

char *GetDateTimeStr(char *buffer, size_t length){
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)){
    strcpy(buffer, "Failed to otain time");
  } else {
    strftime(buffer, length, "%Y-%m-%d_%H-%M-S_%a_%b", &timeInfo);
  }
  return buffer;
}