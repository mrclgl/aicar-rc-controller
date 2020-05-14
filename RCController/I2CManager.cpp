#include "I2CManager.h"

#define I2C_RECEIVE_MESSAGE_BUFFER_SIZE 9 //Max expected read size
#define I2C_SEND_MESSAGE_BUFFER_SIZE 9 //Max write message size

void OnReceiveI2C(int numBytes);
void OnRequestI2C();

volatile uint8_t i2cGetCommand = 0;
volatile uint8_t i2cSetCommand = 0;
volatile uint8_t* i2cSetCommandDataBuffer = new uint8_t[I2C_RECEIVE_MESSAGE_BUFFER_SIZE];
volatile uint8_t i2cSetCommandDataBufferSize = 0;

void I2CManager::begin(uint8_t i2cAddress, StateManager* stateManager, RCReceiver* rcReceiver, TemperatureSensor* motorTemp, AutomaticFan* motorFan)
{
    if (started) return;
    started = true;

    this->stateManager = stateManager;
    this->rcReceiver = rcReceiver;
    this->motorTemp = motorTemp;
    this->motorFan = motorFan;

    Wire.begin(i2cAddress);
    Wire.onReceive(OnReceiveI2C);
    Wire.onRequest(OnRequestI2C);
}

long t = 0;
uint16_t n = 0;

void I2CManager::loop()
{
    if (i2cSetCommand & (1 << 7))
    {
        uint8_t index = i2cSetCommand & ~(1 << 7);

        switch(index)
        {
            case 0:
                handleSetModeOfOperation(const_cast<uint8_t*>(i2cSetCommandDataBuffer), i2cSetCommandDataBufferSize);
                break;
            case 1:
                handleSetRCControlOutput(const_cast<uint8_t*>(i2cSetCommandDataBuffer), i2cSetCommandDataBufferSize);
                break;
            case 2:
                handleSetMotorFanSettings(const_cast<uint8_t*>(i2cSetCommandDataBuffer), i2cSetCommandDataBufferSize);
                break;
        }

        i2cSetCommand = 0;
    }
}

StateManager* I2CManager::getStateManager()
{
    return stateManager;
}

RCReceiver* I2CManager::getRCReceiver()
{
    return rcReceiver;
}

TemperatureSensor* I2CManager::getMotorTemp()
{
    return motorTemp;
}

AutomaticFan* I2CManager::getMotorFan()
{
    return motorFan;
}

/**
 * Message Structure:
 * 
 * byte 1:
 *      - Bit 7 not set:    Get
 *      - Bit 7 set:        Set
 *      - Bits 0-6:         Index
 * 
 * byte 2-end: 
 *      - Data
 **/

void I2CManager::handleSetModeOfOperation(uint8_t* data, uint8_t size)
{
    if (size < 1) return;

    Serial.print("Requesting ModeOfOperation ");
    Serial.println(data[0]);

    stateManager->requestModeOfOperation(data[0]);
}

void I2CManager::handleSetRCControlOutput(uint8_t* data, uint8_t size)
{
    if (size < 4) return;

    uint16_t throttlePulse = *static_cast<uint16_t*>(static_cast<void*>(data));
    throttlePulse = min(max(1000, throttlePulse), 2000);

    Serial.print("Set Throttle Pulse: ");
    Serial.println(throttlePulse);

    //rcReceiver->setThrottleOutPulseMicros(throttlePulse);

    uint16_t steeringPulse = *static_cast<uint16_t*>(static_cast<void*>(data+2));
    steeringPulse = min(max(1000, steeringPulse), 2000);

    Serial.print("Set Steering Pulse: ");
    Serial.println(steeringPulse);

    rcReceiver->setSteeringOutPulseMicros(steeringPulse);
}

void I2CManager::handleSetMotorFanSettings(uint8_t* data, uint8_t size)
{
    if (size < 8) return;

    uint16_t updateIntervalMillis = *static_cast<uint16_t*>(static_cast<void*>(data));

    float fanOffTemp = (*static_cast<int16_t*>(static_cast<void*>(data+2))/100.0f);
    float fanMaxTemp = (*static_cast<int16_t*>(static_cast<void*>(data+4))/100.0f);

    bool manualOverride = *(data+6) > 0 ? true : false;
    uint8_t manualSpeed = *(data+7);

    Serial.println("Set MotorFanInfo:");

    Serial.print("updateIntervalMillis: ");
    Serial.println(updateIntervalMillis);
    Serial.print("fanOffTemp: ");
    Serial.println(fanOffTemp);
    Serial.print("fanMaxTemp: ");
    Serial.println(fanMaxTemp);
    Serial.print("manualOverride: ");
    Serial.println(manualOverride);
    Serial.print("manualSpeed: ");
    Serial.println(manualSpeed);

    motorFan->setUpdateIntervalMillis(updateIntervalMillis);
    motorFan->setFanOffTemp(fanOffTemp);
    motorFan->setFanMaxTemp(fanMaxTemp);
    motorFan->setManualOverride(manualOverride);
    motorFan->setManualSpeed(manualSpeed);
}

void OnReceiveI2C(int numBytes)
{
    if (numBytes < 1) return;

    byte cmd = Wire.read();

    if (cmd & (1 << 7))
    {//Highest bit set => Set-Command
        if ((i2cSetCommand & (1 << 7)) == 0)
        {//We don't have a Set-Command to execute
            i2cSetCommand = cmd;
        }
        else
        {//We already have a Set-Command to execute, so ignore this one and wait for the first one to finish
            while(Wire.available())
                Wire.read();

            return;
        }
    }
    else
    {//Highest bit not set => Get-Command
        i2cGetCommand = (cmd | (1 << 7));
    }

    i2cSetCommandDataBufferSize = min(numBytes-1, I2C_RECEIVE_MESSAGE_BUFFER_SIZE);
    for(uint8_t i = 0; Wire.available() && i < i2cSetCommandDataBufferSize; i++)
    {
        i2cSetCommandDataBuffer[i] = Wire.read();
    }
}

void OnRequestI2C()
{
    if (i2cGetCommand & (1 << 7))
    {
        switch(i2cGetCommand & ~(1 << 7))
        {
            case 0://ModeOfOperation
            {
                uint8_t modeOfOperation = I2CManager::getInstance().getStateManager()->getCurrentModeOfOperation();
                Wire.write(&modeOfOperation, 1);
            }
            break;
            case 1://RCControlInput
            {
                uint16_t throttlePulse = I2CManager::getInstance().getRCReceiver()->getThrottleInPulseMicros();
                uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(&throttlePulse));
                Wire.write(data, 2);

                uint16_t steeringPulse = I2CManager::getInstance().getRCReceiver()->getSteeringInPulseMicros();
                data = static_cast<uint8_t*>(static_cast<void*>(&steeringPulse));
                Wire.write(data, 2);
            }
            break;
            case 2://MotorFanInfo
            {
                uint8_t fanSpeed = I2CManager::getInstance().getMotorFan()->getCurrentFanSpeed();
                Wire.write(&fanSpeed, 1);

                uint16_t updateIntervalMillis = I2CManager::getInstance().getMotorFan()->getUpdateIntervalMillis();
                uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(&updateIntervalMillis));
                Wire.write(data, 2);

                int16_t fanOffTemp = I2CManager::getInstance().getMotorFan()->getFanOffTemp() * 100;
                data = static_cast<uint8_t*>(static_cast<void*>(&fanOffTemp));
                Wire.write(data, 2);

                int16_t fanMaxTemp = I2CManager::getInstance().getMotorFan()->getFanMaxTemp() * 100;
                data = static_cast<uint8_t*>(static_cast<void*>(&fanMaxTemp));
                Wire.write(data, 2);

                uint8_t manualOverride = I2CManager::getInstance().getMotorFan()->getManualOverride();
                Wire.write(&manualOverride, 1);

                uint8_t manualSpeed = I2CManager::getInstance().getMotorFan()->getManualSpeed();
                Wire.write(&manualSpeed, 1);
            }
            break;
            case 3://MotorTemp
            {
                int16_t currentTempC = I2CManager::getInstance().getMotorTemp()->getCurrentTempC() * 100;
                uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(&currentTempC));
                Wire.write(data, 2);
            }
            break;
        }
    }
}