#ifndef RC_CONTROLLER_AUTOMATIC_FAN
#define RC_CONTROLLER_AUTOMATIC_FAN

#include <Arduino.h>
#include <EEPROM.h>

#include "TemperatureSensor.h"

class AutomaticFan{

public:
    //Needs 12 bytes in EEPROM
    AutomaticFan(uint8_t pin, TemperatureSensor* sensor, uint16_t eepromOffset);
    void begin(bool lowerPwmFrequency = false);
    void loop();
    void resetEEPROM();
    uint8_t getCurrentFanSpeed();
    uint16_t getUpdateIntervalMillis();
    float getFanOffTemp();
    float getFanMaxTemp();
    bool getManualOverride();
    uint8_t getManualSpeed();
    void setUpdateIntervalMillis(uint16_t interval);
    void setFanOffTemp(float offTemp);
    void setFanMaxTemp(float maxTemp);
    void setManualOverride(bool override);
    void setManualSpeed(uint8_t speed);

private:
    String getEEPROMKey(String baseKey);
    void setFanSpeed(uint8_t speed);
    void updateManualSpeed();

    uint8_t pin;
    TemperatureSensor* sensor;
    uint16_t eepromOffset;

    bool started;
    long nextUpdateMillis;

    uint8_t currentFanSpeed;
    uint16_t updateIntervalMillis;
    float fanOffTemp;
    float fanMaxTemp;
    bool manualOverride;
    uint8_t manualSpeed;
};

#endif