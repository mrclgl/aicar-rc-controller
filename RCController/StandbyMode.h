#ifndef RC_CONTROLLER_STANDBY_MODE
#define RC_CONTROLLER_STANDBY_MODE

#include "ModeOfOperation.h"

class StandbyMode : public ModeOfOperation
{

public:
    StandbyMode(RCReceiver* rcReceiver, RGBLed* statusLed);
    void begin();
    void loop();
    void end();

private:

};

#endif