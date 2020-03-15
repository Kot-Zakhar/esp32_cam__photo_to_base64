#include "EmailController.h"

SMTPData smtpData;

void initEmailController(){
}

bool sendLetter(const char *subject, const char *message, const char *attachmentPath, bool isHtml, bool retryUntilSuccess){
  log_d("Sending letter");

  char *server = PERSONAL_SMTP_SERVER;
  char *port = PERSONAL_SMTP_PORT;
	char *login = PERSONAL_SMTP_LOGIN;
  char *password = PERSONAL_SMTP_PASSWORD;
  char *sender = PERSONAL_EMAIL_SENDER;
  char *recipient = PERSONAL_EMAIL_RECIPIENT;

  log_d("Email:\n ---\n Smtp server: %s\n Smtp port: %s\n Smtp login: %s\n Smtp sender name: %s\n ---\n Recipient: %s\n Subject: %s\n Message:\n %s\n Attachement: %s\n ---\n",
    server,
    port,
    login,
    sender,
    recipient,
    subject,
    message,
    attachmentPath
  );

  smtpData.setDebug(true);

  smtpData.setLogin(server, String(port).toInt(), login, password);
  smtpData.setSender(sender, login);
  smtpData.setPriority("High");
  smtpData.setSubject(subject);
  smtpData.setMessage(message, isHtml);
  smtpData.addRecipient(recipient);
  smtpData.setFileStorageType(MailClientStorageType::SD);
  if (attachmentPath != NULL) 
    smtpData.addAttachFile(attachmentPath);

  bool result = false;
  do {
    log_v("Trying to send email..." );
    result = MailClient.sendMail(smtpData);
    if (result)
      log_v("Successfully sent.");
    else
      log_v("Not sent.");
  } while(!result && retryUntilSuccess);

  smtpData.empty();

  return result;
}
