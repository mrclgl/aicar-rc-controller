#ifndef RC_CONTROLLER_I2C_MANAGER
#define RC_CONTROLLER_I2C_MANAGER

class StateManager;
class RCReceiver;
class TemperatureSensor;
class AutomaticFan;

#include <Arduino.h>

class I2CManager
{

private:
    I2CManager() {}
    I2CManager(I2CManager const&);
    void operator=(I2CManager const&);

public:
    static I2CManager& getInstance()
    {
        static I2CManager instance;
        return instance;
    }
    void begin(uint8_t i2cAddress, StateManager* stateManager, RCReceiver* rcReceiver, TemperatureSensor* motorTemp, AutomaticFan* motorFan);
    void loop();
    StateManager* getStateManager();
    RCReceiver* getRCReceiver();
    TemperatureSensor* getMotorTemp();
    AutomaticFan* getMotorFan();

private:

    void handleSetModeOfOperation(uint8_t* data, uint8_t size);
    void handleSetRCControlOutput(uint8_t* data, uint8_t size);
    void handleSetMotorFanSettings(uint8_t* data, uint8_t size);

    bool started;
    volatile bool i2cMessageReceivedFlag;

    StateManager* stateManager;
    RCReceiver* rcReceiver;
    TemperatureSensor* motorTemp;
    AutomaticFan* motorFan;
};

#endif