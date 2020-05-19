#include "StandbyMode.h"

#include "StateManager.h"
#include "RCReceiver.h"
#include "RGBLed.h"

StandbyMode::StandbyMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed) : ModeOfOperation(stateManager, rcReceiver, statusLed) {}

void StandbyMode::begin()
{
    statusLed->setRGB(RGBLedColor::Black);
    long now = millis();
    statusLed->setPulseR(75, 925, now + 925);
    statusLed->setPulseG(75, 925, now + 925);
}