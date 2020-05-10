#include "I2CManager.h"

void OnReceiveI2C(int numBytes);

I2CManager::I2CManager(StateManager* stateManager, TemperatureSensor* motorTemp, AutomaticFan* motorFan)
{
    this->stateManager = stateManager;
    this->motorTemp = motorTemp;
    this->motorFan = motorFan;
}

void I2CManager::begin(uint8_t i2cAddress)
{
    Wire.begin(i2cAddress);
    Wire.onReceive(OnReceiveI2C);
}

void I2CManager::loop()
{

}

void OnReceiveI2C(int numBytes)
{
    
}