#ifndef RC_CONTROLLER_STATE_MANAGER
#define RC_CONTROLLER_STATE_MANAGER

#include <Arduino.h>

#include "RCReceiver.h"
#include "RGBLed.h"

class StateManager
{

public:
    StateManager(RCReceiver* rcReceiver, RGBLed* rgbLed);
    void begin();
    void loop();

private:
    RCReceiver* rcReceiver;
    RGBLed* rgbLed;

};

#endif