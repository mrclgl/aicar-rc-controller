#include "StandbyMode.h"

StandbyMode::StandbyMode(RCReceiver* rcReceiver, RGBLed* statusLed) : ModeOfOperation(rcReceiver, statusLed) {}

void StandbyMode::begin()
{
    statusLed->setRGB(false, true, false);
}

void StandbyMode::loop()
{

}

void StandbyMode::end()
{

}