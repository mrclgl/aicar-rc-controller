#ifndef RC_CONTROLLER_MODE_OF_OPERATION
#define RC_CONTROLLER_MODE_OF_OPERATION

class StateManager;
class RCReceiver;
class RGBLed;

#include <Arduino.h>

class ModeOfOperation
{

public:
    ModeOfOperation(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed);
    virtual void begin() {}
    virtual void loop() {}
    virtual void end() {}

protected:
    StateManager* stateManager;
    RCReceiver* rcReceiver;
    RGBLed* statusLed;

};

#endif