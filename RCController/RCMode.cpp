#include "RCMode.h"

#include "StateManager.h"
#include "RCReceiver.h"
#include "RGBLed.h"

#define DEBUG 0
#define DISABLE_RC_THROTTLE 0
#define DISABLE_RC_STEERING 0

RCMode::RCMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed) : ModeOfOperation(stateManager, rcReceiver, statusLed) {}

void RCMode::begin()
{
    RGBLedColor color = stateManager->getModeColor(ModesOfOperation::RC);
    statusLed->setRGB(color);
}

#if DEBUG
long lastRCModeSerial = 0;
#endif

void RCMode::loop()
{
    #if !DISABLE_RC_THROTTLE
    rcReceiver->setThrottleOutPulseMicros(rcReceiver->getThrottleInPulseMicros());
    #endif

    #if !DISABLE_RC_STEERING
    rcReceiver->setSteeringOutPulseMicros(rcReceiver->getSteeringInPulseMicros());
    #endif

    #if DEBUG
    if (millis() - lastRCModeSerial >= 250)
    {
        Serial.print("THR: ");
        Serial.print(rcReceiver->getThrottleInPulseMicros());
        Serial.print(" - STR: ");
        Serial.println(rcReceiver->getSteeringInPulseMicros());
        lastRCModeSerial = millis();
    }
    #endif
}

void RCMode::end()
{
    rcReceiver->setReceiverPower(false);
}