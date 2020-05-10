#ifndef RC_CONTROLLER_I2C_MANAGER
#define RC_CONTROLLER_I2C_MANAGER

#include <Arduino.h>
#include <Wire.h>

#include "TemperatureSensor.h"
#include "AutomaticFan.h"
#include "StateManager.h"

class I2CManager
{

public:
    I2CManager(StateManager* stateManager, TemperatureSensor* motorTemp, AutomaticFan* motorFan);
    void begin(uint8_t i2cAddress);
    void loop();
    
private:
    StateManager* stateManager;
    TemperatureSensor* motorTemp;
    AutomaticFan* motorFan;
};

#endif