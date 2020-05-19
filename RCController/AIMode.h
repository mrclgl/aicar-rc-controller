#ifndef RC_CONTROLLER_AI_MODE
#define RC_CONTROLLER_AI_MODE

class ModeOfOperation;
class StateManager;
class RCReceiver;
class RGBLed;

#include "ModeOfOperation.h"
#include "Types.h"

class AIMode : public ModeOfOperation
{

public:
    AIMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed);
    void begin();
    void loop();
    void end();

private:
    long modeCancelMillis;

};

#endif