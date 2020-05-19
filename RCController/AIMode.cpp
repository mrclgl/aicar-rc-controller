#include "AIMode.h"

#include "StateManager.h"
#include "RCReceiver.h"
#include "RGBLed.h"

#define MODE_CANCEL_THRESHOLD_THROTTLE_PULSE_MILLIS 1400

#define STATUS_LED_MODE_CANCEL_COLOR RGBLedColor::Red
#define STATUS_LED_MODE_CANCEL_FLASH_INTERVAL_MILLIS 50

#define MODE_CANCEL_RETURN_DURATION_MILLIS 2000

AIMode::AIMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed) : ModeOfOperation(stateManager, rcReceiver, statusLed) {}

void AIMode::begin()
{
    modeCancelMillis = -1;

    RGBLedColor color = stateManager->getModeColor(ModesOfOperation::AI);
    statusLed->setRGB(color);
}

void AIMode::loop()
{
    if (modeCancelMillis == -1)
    {
        if (rcReceiver->getThrottleInPulseMicros() <= MODE_CANCEL_THRESHOLD_THROTTLE_PULSE_MILLIS)
        {
            rcReceiver->setReceiverPower(false);
            modeCancelMillis = millis();

            statusLed->setRGBPulse(STATUS_LED_MODE_CANCEL_COLOR, STATUS_LED_MODE_CANCEL_FLASH_INTERVAL_MILLIS, STATUS_LED_MODE_CANCEL_FLASH_INTERVAL_MILLIS, 0);
        }
    }
    else
    {
        if (millis() - modeCancelMillis >= MODE_CANCEL_RETURN_DURATION_MILLIS)
        {
            stateManager->setModeOfOperation(ModesOfOperation::Standby);
            return;
        }
    }
}

void AIMode::end()
{
    rcReceiver->setReceiverPower(false);
}