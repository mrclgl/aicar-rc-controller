#include "RGBLed.h"

RGBLed::RGBLed(uint8_t pinR, uint8_t pinG, uint8_t pinB)
{
    this->pinR = pinR;
    this->pinG = pinG;
    this->pinB = pinB;
}

void RGBLed::begin()
{
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
}

void RGBLed::loop()
{
    long now = millis();

    if (nextToggleMillisR > -1 && now >= nextToggleMillisR)
    {
        PINB  = (1 << pinR - 8);

        bool state = getState(0);
        nextToggleMillisR = now + (state ? offTimeR : onTimeR);
        toggleState(0);
    }

    if (nextToggleMillisG > -1 && now >= nextToggleMillisG)
    {
        PINB = (1 << pinG - 8);
        
        bool state = getState(1);
        nextToggleMillisG = now + (state ? offTimeG : onTimeG);
        toggleState(1);
    }

    if (nextToggleMillisB > -1 && now >= nextToggleMillisB)
    {
        PINB = (1 << pinB - 8);
        
        bool state = getState(2);
        nextToggleMillisB = now + (state ? offTimeB : onTimeB);
        toggleState(2);
    }
}

void RGBLed::setRGB(bool red, bool green, bool blue)
{
    digitalWrite(pinR, red ? HIGH : LOW);
    digitalWrite(pinG, green ? HIGH : LOW);
    digitalWrite(pinB, blue ? HIGH : LOW);

    nextToggleMillisR = -1;
    nextToggleMillisG = -1;
    nextToggleMillisB = -1;
}

void RGBLed::setPulseR(uint16_t onTime, uint16_t offTime, long startMillis)
{
    onTimeR = onTime;
    offTimeR = offTime;
    nextToggleMillisR = startMillis > -1 ? startMillis : 0;

    setState(0, false);
}

void RGBLed::setPulseG(uint16_t onTime, uint16_t offTime, long startMillis)
{
    onTimeG = onTime;
    offTimeG = offTime;
    nextToggleMillisG = startMillis > -1 ? startMillis : 0;

    setState(1, false);
}

void RGBLed::setPulseB(uint16_t onTime, uint16_t offTime, long startMillis)
{
    onTimeB = onTime;
    offTimeB = offTime;
    nextToggleMillisB = startMillis > -1 ? startMillis : 0;

    setState(2, false);
}

bool RGBLed::getState(uint8_t index)
{
    return states & (1 << index);
}

void RGBLed::setState(uint8_t index, bool state)
{
    if (state)
        states |= (1 << index);
    else
        states &= ~(1 << index);
}

void RGBLed::toggleState(uint8_t index)
{
    states ^= (1 << index);
}