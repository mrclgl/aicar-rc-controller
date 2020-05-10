#include "RCReceiver.h"

RCReceiver::RCReceiver(uint8_t pinPower, uint8_t pinThrottleIn, uint8_t pinThrottleOut, uint8_t pinSteeringIn, uint8_t pinSteeringOut)
{
    this->pinPower = pinPower;
    this->pinThrottleIn = pinThrottleIn;
    this->pinThrottleOut = pinThrottleOut;
    this->pinSteeringIn = pinSteeringIn;
    this->pinSteeringOut = pinSteeringOut;
}

void RCReceiver::begin()
{
    pinMode(pinPower, OUTPUT);
    setReceiverPower(false);

    rcReader.begin();
    rcReader.addChannel(pinThrottleIn);
    rcReader.addChannel(pinSteeringIn);
}

void RCReceiver::loop()
{

}

uint16_t RCReceiver::getThrottleInPulseMicros()
{
    return rcReader.getFreq(pinThrottleIn);
}

uint16_t RCReceiver::getSteeringInPulseMicros()
{
    return rcReader.getFreq(pinSteeringIn);
}

bool RCReceiver::getReceiverPower()
{
    return receiverPower;
}

void RCReceiver::setReceiverPower(bool power)
{
    digitalWrite(pinPower, power ? HIGH : LOW);

    if (power)
    {
        throttleServo.writeMicroseconds(1500);
        throttleServo.attach(pinThrottleOut);

        steeringServo.writeMicroseconds(1500);
        steeringServo.attach(pinSteeringOut);
    }
    else
    {
        throttleServo.detach();
        steeringServo.detach();
    }

    receiverPower = power;
}

void RCReceiver::setThrottleOutPulseMicros(uint16_t pulseMicros)
{
    throttleServo.writeMicroseconds(pulseMicros);
}

void RCReceiver::setSteeringOutPulseMicros(uint16_t pulseMicros)
{
    steeringServo.writeMicroseconds(pulseMicros);
}