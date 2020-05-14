#ifndef RC_CONTROLLER_MODE_OF_OPERATION
#define RC_CONTROLLER_MODE_OF_OPERATION

#include <Arduino.h>

#include "RCReceiver.h"
#include "RGBLed.h"

class ModeOfOperation
{

public:
    ModeOfOperation(RCReceiver* rcReceiver, RGBLed* statusLed);
    virtual void begin();
    virtual void loop();
    virtual void end();

protected:
    RCReceiver* rcReceiver;
    RGBLed* statusLed;

};

#endif