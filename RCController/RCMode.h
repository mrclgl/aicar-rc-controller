#ifndef RC_CONTROLLER_RC_MODE
#define RC_CONTROLLER_RC_MODE

class ModeOfOperation;
class StateManager;
class RCReceiver;
class RGBLed;

#include "ModeOfOperation.h"
#include "Types.h"

class RCMode : public ModeOfOperation
{

public:
    RCMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed);
    void begin();
    void loop();
    void end();

private:


};

#endif