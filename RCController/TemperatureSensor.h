#ifndef RC_CONTROLLER_TEMPERATURE_SENSOR
#define RC_CONTROLLER_TEMPERATURE_SENSOR

#include <Arduino.h>
#include <Wire.h>
#include <DallasTemperature.h>

#define INVALID_TEMPERATURE_VALUE -1000.0f

class TemperatureSensor
{

public:
    TemperatureSensor(uint8_t pin);
    void begin();
    void loop();
    bool isCurrentTempValid();
    float getCurrentTempC();

private:
    OneWire oneWire;
    DallasTemperature sensor;
    bool started;
    int16_t millisToWaitForConversion;
    long millisConversionComplete;
    float currentTempC;

};

#endif