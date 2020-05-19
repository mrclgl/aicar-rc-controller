#include "ModeOfOperation.h"

#include "StateManager.h"
#include "RCReceiver.h"
#include "RGBLed.h"

ModeOfOperation::ModeOfOperation(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed)
{
    this->stateManager = stateManager;
    this->rcReceiver = rcReceiver;
    this->statusLed = statusLed;
}