#ifndef RC_CONTROLLER_RGB_LED
#define RC_CONTROLLER_RGB_LED

#include <Arduino.h>
#include "Types.h"

class RGBLed
{

public:
    RGBLed(uint8_t pinR, uint8_t pinG, uint8_t pinB);
    void begin();
    void loop();
    void setRGB(RGBLedColor color);
    void setPulseR(uint16_t onTime, uint16_t offTime, long startMillis);
    void setPulseG(uint16_t onTime, uint16_t offTime, long startMillis);
    void setPulseB(uint16_t onTime, uint16_t offTime, long startMillis);
    void setRGBPulse(RGBLedColor color, uint16_t onTime, uint16_t offTime, long startMillis);

private:
    bool getState(uint8_t index);
    void setState(uint8_t index, bool state);
    void toggleState(uint8_t index);
    uint8_t pinR;
    uint8_t pinG;
    uint8_t pinB;

    unsigned char states;//0-2: On or Off
    uint16_t onTimeR;
    uint16_t offTimeR;
    long nextToggleMillisR;
    uint16_t onTimeG;
    uint16_t offTimeG;
    long nextToggleMillisG;
    uint16_t onTimeB;
    uint16_t offTimeB;
    long nextToggleMillisB;
};


#endif