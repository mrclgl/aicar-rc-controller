#include "StateManager.h"

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
    
}

ModesOfOperation StateManager::getCurrentModeOfOperation()
{
    return currentModeOfOperation;
}

void StateManager::requestModeOfOperation(uint8_t mode)
{

}

void StateManager::setModeOfOperation(ModesOfOperation mode)
{
    if (currentMode != nullptr)
    {
        currentMode->end();
        delete currentMode;
    }

    switch (mode)
    {
        /*case ModesOfOperation::RC:
        {

        }
        break;

        case ModesOfOperation::AI:
        {

        }
        break;*/

        default:
        {
            currentMode = new StandbyMode(rcReceiver, statusLed);
        }
        break;
    }

    currentModeOfOperation = mode;
    currentMode->begin();
}