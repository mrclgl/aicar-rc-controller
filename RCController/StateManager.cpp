#include "StateManager.h"

StateManager::StateManager(RCReceiver* rcReceiver, RGBLed* rgbLed)
{
    this->rcReceiver = rcReceiver;
    this->rgbLed = rgbLed;
}