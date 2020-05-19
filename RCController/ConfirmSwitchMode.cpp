#include "ConfirmSwitchMode.h"

#include "StateManager.h"
#include "RCReceiver.h"
#include "RGBLed.h"

#define DEBUG 0

#define STATUS_LED_CONFIRM_FLASH_INTERVAL_MILLIS 500
#define STATUS_LED_RELEASE_FLASH_INTERVAL_MILLIS 75

#define STATUS_LED_TIMEOUT_FLASH_INTERVAL_MILLIS 150
#define STATUS_LED_TIMEOUT_COLOR RGBLedColor::Magenta

#define CONFIRM_TIMEOUT_MILLIS 10000
#define CONFIRM_TIMEOUT_RETURN_DURATION_MILLIS 2000

#define CENTER_PULSE_MICROS 1500

#define DEADZONE_PULSE_MICROS 100

#define CONFIRM_THROTTLE_PULSE_MICROS 2000
#define CONFIRM_STEERING_PULSE_MICROS 2000
#define CONFIRM_HOLD_DURATION_MILLIS 1000
#define RELEASE_HOLD_DURATION_MILLIS 500

ConfirmSwitchMode::ConfirmSwitchMode(StateManager* stateManager, RCReceiver* rcReceiver, RGBLed* statusLed, ModesOfOperation newMode) : ModeOfOperation(stateManager, rcReceiver, statusLed)
{
    this->newMode = newMode;
}

void ConfirmSwitchMode::begin()
{
    rcReceiver->setReceiverPower(true);

    modeStartMillis = millis();
    confirmHoldStartMillis = -1;
    releaseHoldStartMillis = -1;

    updateStatus(ConfirmSwitchModeStates::Confirm);
}

#if DEBUG
long lastConfirmSwitchSerial = 0;
#endif

void ConfirmSwitchMode::loop()
{
    switch (currentState)
    {
        case ConfirmSwitchModeStates::Confirm:
        {
            if (millis() - modeStartMillis >= CONFIRM_TIMEOUT_MILLIS)
            {
                updateStatus(ConfirmSwitchModeStates::Timeout);
                break;
            }
            
            #if DEBUG
            if (millis() - lastConfirmSwitchSerial >= 250)
            {
                Serial.print("THR: ");
                Serial.print(abs(CONFIRM_THROTTLE_PULSE_MICROS - (int16_t)rcReceiver->getThrottleInPulseMicros()));
                Serial.print(" <= ");
                Serial.print(DEADZONE_PULSE_MICROS);
                Serial.print(" - STR: ");
                Serial.print(abs(CONFIRM_STEERING_PULSE_MICROS - (int16_t)rcReceiver->getSteeringInPulseMicros()));
                Serial.print(" <= ");
                Serial.println(DEADZONE_PULSE_MICROS);
                lastConfirmSwitchSerial = millis();
            }
            #endif

            if (abs(CONFIRM_THROTTLE_PULSE_MICROS - (int16_t)rcReceiver->getThrottleInPulseMicros()) <= DEADZONE_PULSE_MICROS  && 
            abs(CONFIRM_STEERING_PULSE_MICROS - (int16_t)rcReceiver->getSteeringInPulseMicros()) <= DEADZONE_PULSE_MICROS)
            {
                if (confirmHoldStartMillis == -1)
                {
                    confirmHoldStartMillis = millis();
                    #if DEBUG
                    Serial.println("HOLD");
                    #endif
                }
                else if (millis() - confirmHoldStartMillis >= CONFIRM_HOLD_DURATION_MILLIS)
                {
                    updateStatus(ConfirmSwitchModeStates::Release);
                }
            }
            else if (confirmHoldStartMillis > -1)
            {
                confirmHoldStartMillis = -1;
            }
        }
        break;

        case ConfirmSwitchModeStates::Release:
        {

            #if DEBUG
            if (millis() - lastConfirmSwitchSerial >= 250)
            {
                Serial.print("THR: ");
                Serial.print(abs(CENTER_PULSE_MICROS - (int16_t)rcReceiver->getThrottleInPulseMicros()));
                Serial.print(" <= ");
                Serial.print(DEADZONE_PULSE_MICROS);
                Serial.print(" - STR: ");
                Serial.print(abs(CENTER_PULSE_MICROS - (int16_t)rcReceiver->getSteeringInPulseMicros()));
                Serial.print(" <= ");
                Serial.println(DEADZONE_PULSE_MICROS);
                lastConfirmSwitchSerial = millis();
            }
            #endif

            if (abs(CENTER_PULSE_MICROS - (int16_t)rcReceiver->getThrottleInPulseMicros()) <= DEADZONE_PULSE_MICROS && 
            abs(CENTER_PULSE_MICROS - (int16_t)rcReceiver->getSteeringInPulseMicros()) <= DEADZONE_PULSE_MICROS)
            {
                if (releaseHoldStartMillis == -1)
                {
                    releaseHoldStartMillis = millis();
                }
                else if (millis() - releaseHoldStartMillis >= RELEASE_HOLD_DURATION_MILLIS)
                {
                    finish(true);
                }
            }
            else if (releaseHoldStartMillis > -1)
            {
                releaseHoldStartMillis = -1;
            }
        }
        break;

        case ConfirmSwitchModeStates::Timeout:
        {
            if (millis() - modeStartMillis >= (CONFIRM_TIMEOUT_MILLIS + CONFIRM_TIMEOUT_RETURN_DURATION_MILLIS))
            {
                finish(false);
            }
        }
        break;
    }
}

void ConfirmSwitchMode::end()
{
    statusLed->setRGB(RGBLedColor::Black);

    if (currentState == ConfirmSwitchModeStates::None || 
    currentState == ConfirmSwitchModeStates::Timeout || 
    (newMode != ModesOfOperation::RC && newMode != ModesOfOperation::AI))
    {
        rcReceiver->setReceiverPower(false);
    }
}

void ConfirmSwitchMode::updateStatus(ConfirmSwitchModeStates state)
{
    switch (state)
    {
        case ConfirmSwitchModeStates::Confirm:
        {
            #if DEBUG
            Serial.println("ConfirmSwitchState: CONFIRM");
            #endif

            RGBLedColor color = stateManager->getModeColor(newMode);
            statusLed->setRGBPulse(color, STATUS_LED_CONFIRM_FLASH_INTERVAL_MILLIS, STATUS_LED_CONFIRM_FLASH_INTERVAL_MILLIS, 0);

            currentState = ConfirmSwitchModeStates::Confirm;
        }
        break;

        case ConfirmSwitchModeStates::Release:
        {
            #if DEBUG
            Serial.println("ConfirmSwitchState: RELEASE");
            #endif

            RGBLedColor color = stateManager->getModeColor(newMode);
            statusLed->setRGBPulse(color, STATUS_LED_RELEASE_FLASH_INTERVAL_MILLIS, STATUS_LED_RELEASE_FLASH_INTERVAL_MILLIS, 0);

            currentState = ConfirmSwitchModeStates::Release;
        }
        break;

        case ConfirmSwitchModeStates::Timeout:
        {
            #if DEBUG
            Serial.println("ConfirmSwitchState: TIMEOUT");
            #endif
            
            statusLed->setRGBPulse(STATUS_LED_TIMEOUT_COLOR, STATUS_LED_TIMEOUT_FLASH_INTERVAL_MILLIS, STATUS_LED_TIMEOUT_FLASH_INTERVAL_MILLIS, 0);

            currentState = ConfirmSwitchModeStates::Timeout;
        }
        break;
    }
}

void ConfirmSwitchMode::finish(bool confirmed)
{
    if (confirmed)
    {
        stateManager->setModeOfOperation(newMode, false);
    }
    else
    {
        stateManager->setModeOfOperation(ModesOfOperation::Standby, false);
    }
}