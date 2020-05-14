#include "ModeOfOperation.h"

ModeOfOperation::ModeOfOperation(RCReceiver* rcReceiver, RGBLed* statusLed)
{
    this->rcReceiver = rcReceiver;
    this->statusLed = statusLed;
}