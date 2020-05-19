#ifndef RC_CONTROLLER_STATE_MANAGER
#define RC_CONTROLLER_STATE_MANAGER

class RCReceiver;
class RGBLed;
class ModeOfOperation;

#include <Arduino.h>
#include "Types.h"

class StateManager
{

public:
    StateManager(RCReceiver* rcReceiver, RGBLed* statusLed);
    void begin();
    void loop();
    ModesOfOperation getCurrentModeOfOperation();
    void requestModeOfOperation(ModesOfOperation mode);
    void setModeOfOperation(ModesOfOperation mode, bool needsConfirm = false);
    RGBLedColor getModeColor(ModesOfOperation mode);

private:
    RCReceiver* rcReceiver;
    RGBLed* statusLed;

    ModesOfOperation currentModeOfOperation;
    ModeOfOperation* currentMode;
};

#endif