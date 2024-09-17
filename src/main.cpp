#include <Arduino.h>
#include "MPU6050.h"
MPU6050 jsp;
 void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("Bonjour \n\r");
  jsp.init();
}

void loop()
{
  jsp.TEST();
  delay(10);
}
