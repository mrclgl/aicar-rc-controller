#ifndef RC_CONTROLLER_CONFIRM_SWITCH_MODE
#define RC_CONTROLLER_CONFIRM_SWITCH_MODE

class ModeOfOperation;
class StateManager;
class RCReceiver;
class RGBLed;

#include "ModeOfOperation.h"
#include "Types.h"

enum ConfirmSwitchModeStates
{
    None = 0,
    Confirm = 1,
    Release = 2,
    Timeout = 3
};

class ConfirmSwitchMode : public ModeOfOperation
{

public:
    ConfirmSwitchMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed, ModesOfOperation newMode);
    void begin();
    void loop();
    void end();

private:
    void updateStatus(ConfirmSwitchModeStates status);
    void finish(bool confirmed);

    ConfirmSwitchModeStates currentState;
    ModesOfOperation newMode;

    long modeStartMillis;
    long confirmHoldStartMillis;
    long releaseHoldStartMillis;
};

#endif