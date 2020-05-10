
#include <Wire.h>

#include "AutomaticFan.h"
#include "TemperatureSensor.h"
#include "RCReceiver.h"
#include "RGBLed.h"

TemperatureSensor motorTemp(2);
AutomaticFan motorFan(3, &motorTemp, 0);//12 bytes EEPROM
RCReceiver rcReceiver(12, 4, 5, 7, 6);
RGBLed statusLed(9, 8, 10);

long lastMillis = 0;

void setup() {
  Serial.begin(115200);

  Wire.begin(4);
  Wire.onReceive(receiveEvent);

  motorTemp.begin();
  motorFan.begin(true);
  rcReceiver.begin();
  statusLed.begin();

  statusLed.setRGB(false, false, false);
  long now = millis();
  statusLed.setPulseR(100, 200, now);
  statusLed.setPulseG(100, 200, now + 100);
  statusLed.setPulseB(100, 200, now + 200);

  //rcReceiver.setReceiverPower(true);
}

void loop() {
  motorTemp.loop();

  if (motorTemp.isCurrentTempValid() && millis() - lastMillis > 500){
    Serial.println("THR: " + String(rcReceiver.getThrottleInPulseMicros()) + " - STR: " + String(rcReceiver.getSteeringInPulseMicros()));
    lastMillis = millis();
  }

  motorFan.loop();
  statusLed.loop();
}

void receiveEvent(int howMany)
{
  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
}
