#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(uint8_t pin) : oneWire(pin), sensor(&oneWire)
{
    started = false;
    millisConversionComplete = -1;
    currentTempC = INVALID_TEMPERATURE_VALUE;
}

void TemperatureSensor::begin()
{
    if (started) return;
    started = true;

    sensor.setWaitForConversion(false);
    sensor.begin();

    uint8_t bitResolution = sensor.getResolution();
    millisToWaitForConversion = sensor.millisToWaitForConversion(bitResolution);
}

void TemperatureSensor::loop()
{
    if (millisConversionComplete < 0)
    {
        sensor.requestTemperatures();
        millisConversionComplete = millis() + millisToWaitForConversion;
    }
    else if (millis() >= millisConversionComplete)
    {
        currentTempC = sensor.getTempCByIndex(0);
        millisConversionComplete = -1;
    }
}

bool TemperatureSensor::isCurrentTempValid()
{
    return currentTempC != INVALID_TEMPERATURE_VALUE;
}

float TemperatureSensor::getCurrentTempC()
{
    return currentTempC;
}