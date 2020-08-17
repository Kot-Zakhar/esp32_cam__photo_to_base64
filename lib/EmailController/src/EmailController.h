#ifndef _EMAIL_CONTROLLER_H_
#define _EMAIL_CONTROLLER_H_

#include <ESP32_MailClient.h>
#include <MemoryController.h>
#include <SDCardController.h>
#include "personal_info_smtp.h"

bool sendLetter(const char *subject, const char *message, const char *attachmentPath = NULL, bool isHtml = false, bool retryUntilSuccess = false);


#endif