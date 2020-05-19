#ifndef RC_CONTROLLER_STANDBY_MODE
#define RC_CONTROLLER_STANDBY_MODE

class ModeOfOperation;
class StateManager;
class RCReceiver;
class RGBLed;

#include "ModeOfOperation.h"

class StandbyMode : public ModeOfOperation
{

public:
    StandbyMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed);
    void begin();

private:

};

#endif