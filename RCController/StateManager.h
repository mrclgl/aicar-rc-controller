#ifndef RC_CONTROLLER_STATE_MANAGER
#define RC_CONTROLLER_STATE_MANAGER

#include <Arduino.h>

#include "RCReceiver.h"
#include "RGBLed.h"

#include "ModeOfOperation.h"
#include "StandbyMode.h"

enum ModesOfOperation
{
    Switch = -1,
    Standby = 0,
    RC = 2,
    AI = 3
};

class StateManager
{

public:
    StateManager(RCReceiver* rcReceiver, RGBLed* statusLed);
    void begin();
    void loop();
    ModesOfOperation getCurrentModeOfOperation();
    void requestModeOfOperation(uint8_t mode);

private:
    void setModeOfOperation(ModesOfOperation mode);
    RCReceiver* rcReceiver;
    RGBLed* statusLed;

    ModesOfOperation currentModeOfOperation;
    ModeOfOperation* currentMode;
};

#endif