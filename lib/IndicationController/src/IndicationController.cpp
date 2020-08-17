#include "IndicationController.h"
#include "jled.h"
#include "Ticker.h"

#define BUILT_IN_LED GPIO_NUM_33
#define EXTRA_LED GPIO_NUM_2
#define LED_UPDATE_FREQ 50

Ticker updater;

JLed builtInLed(BUILT_IN_LED);
JLed extraLed(EXTRA_LED);

void IRAM_ATTR LedUpdateInterrupt() {
    builtInLed.Update();
    extraLed.Update();
}

void initIndication() {
    builtInLed.On(); // built-in led is inverted, so this is actually off function
    updater.attach_ms(LED_UPDATE_FREQ, LedUpdateInterrupt);
}

void indicate(indicationState state) {
    switch (state)
    {
        case ConfigMode:
            builtInLed.Off();
        break;
        case BtCommand: 
        break;
        case BtCommandSuccess: 
        break;
        case BtCommandError: 
        break;
        case WorkMode:
            builtInLed.Blink(500, 500).Forever();
        break;
        case ConnectingToWiFi: 
        break;
        case GetReadyForCapture: 
        break;
        case SendingImage: 
        break;
        case SentSuccessfully: 
        break;
        case SentError: 
        break;
    }
}