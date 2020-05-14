
#include "AutomaticFan.h"
#include "TemperatureSensor.h"
#include "RCReceiver.h"
#include "RGBLed.h"
#include "StateManager.h"
#include "I2CManager.h"

TemperatureSensor motorTemp(2);
AutomaticFan motorFan(3, &motorTemp, 0);//12 bytes EEPROM
RCReceiver rcReceiver(12, 4, 5, 7, 6);
RGBLed statusLed(9, 8, 10);

StateManager stateManager(&rcReceiver, &statusLed);
I2CManager& i2cManager = I2CManager::getInstance();

long lastMillis = 0;

void setup() {
  Serial.begin(115200);

  motorTemp.begin();
  motorFan.begin(true);
  rcReceiver.begin();
  statusLed.begin();

  stateManager.begin();
  i2cManager.begin(4, &stateManager, &rcReceiver, &motorTemp, &motorFan);
}

void loop(){
  motorTemp.loop();
  motorFan.loop();
  statusLed.loop();

  stateManager.loop();
  i2cManager.loop();
}
