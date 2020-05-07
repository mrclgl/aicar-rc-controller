
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
}

void loop() {
  
}

void receiveEvent(int howMany)
{
  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
}
