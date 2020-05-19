#include "StateManager.h"

#include "RCReceiver.h"
#include "RGBLed.h"

#include "ModeOfOperation.h"
#include "ConfirmSwitchMode.h"
#include "StandbyMode.h"
#include "RCMode.h"
#include "AIMode.h"

StateManager::StateManager(RCReceiver* rcReceiver, RGBLed* statusLed)
{
    this->rcReceiver = rcReceiver;
    this->statusLed = statusLed;

    currentMode = nullptr;
}

void StateManager::begin()
{
    setModeOfOperation(ModesOfOperation::Standby);
}

void StateManager::loop()
{
    if (currentMode != nullptr)
        currentMode->loop();
}

ModesOfOperation StateManager::getCurrentModeOfOperation()
{
    return currentModeOfOperation;
}

void StateManager::requestModeOfOperation(ModesOfOperation mode)//mode could potentially have an invalid value
{
    switch ((ModesOfOperation)mode)
    {
        case ModesOfOperation::RC:
        case ModesOfOperation::AI:
        {
            setModeOfOperation((ModesOfOperation)mode, true);
        }
        break;
        
        default:
        {
            setModeOfOperation(ModesOfOperation::Standby, false);
        }
        break;
    }
}

void StateManager::setModeOfOperation(ModesOfOperation mode, bool needsConfirm = false)
{
    if (currentMode != nullptr)
    {
        currentMode->end();
        delete currentMode;
    }

    if (needsConfirm)
    {
        currentMode = new ConfirmSwitchMode(this, rcReceiver, statusLed, mode);

        currentModeOfOperation = ModesOfOperation::ConfirmSwitch;
    }
    else
    {
        switch (mode)
        {
            case ModesOfOperation::RC:
            {
                currentMode = new RCMode(this, rcReceiver, statusLed);
            }
            break;

            case ModesOfOperation::AI:
            {
                currentMode = new AIMode(this, rcReceiver, statusLed);
            }
            break;

            default:
            {
                currentMode = new StandbyMode(this, rcReceiver, statusLed);
            }
            break;
        }

        currentModeOfOperation = mode;
    }

    currentMode->begin();
}

RGBLedColor StateManager::getModeColor(ModesOfOperation mode)
{
    switch (mode)
    {
        case ModesOfOperation::Standby:
        return RGBLedColor::Yellow;
        break;

        case ModesOfOperation::RC:
        return RGBLedColor::Cyan;
        break;

        case ModesOfOperation::AI:
        return RGBLedColor::Green;
        break;
    
        default:
        return RGBLedColor::Black;
        break;
    }
}