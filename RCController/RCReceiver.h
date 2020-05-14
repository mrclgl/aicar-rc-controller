#ifndef RC_CONTROLLER_RC_RECEIVER
#define RC_CONTROLLER_RC_RECEIVER

#include <Arduino.h>
#include <Servo.h>
#include <FastRCReader.h>

class RCReceiver
{

public:
    RCReceiver(uint8_t pinPower, uint8_t pinThrottleIn, uint8_t pinThrottleOut, uint8_t pinSteeringIn, uint8_t pinSteeringOut);
    void begin();
    uint16_t getThrottleInPulseMicros();
    uint16_t getSteeringInPulseMicros();
    bool getReceiverPower();
    void setReceiverPower(bool power);
    void setThrottleOutPulseMicros(uint16_t pulseMicros);
    void setSteeringOutPulseMicros(uint16_t pulseMicros);

private:
    uint8_t pinPower;
    uint8_t pinThrottleIn;
    uint8_t pinThrottleOut;
    uint8_t pinSteeringIn;
    uint8_t pinSteeringOut;
    Servo throttleServo;
    Servo steeringServo;
    FastRCReader rcReader;

    bool receiverPower;
};

#endif
