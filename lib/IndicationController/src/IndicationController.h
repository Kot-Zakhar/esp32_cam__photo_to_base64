#ifndef _INDICATION_CONTROLLER_H_
#define _INDICATION_CONTROLLER_H_

enum indicationState {
    ConfigMode,
    BtCommand,
    BtCommandSuccess,
    BtCommandError,
    WorkMode,
    ConnectingToWiFi,
    GetReadyForCapture,
    SendingImage,
    SentSuccessfully,
    SentError
};

void initIndication();
void indicate(indicationState state);

#endif