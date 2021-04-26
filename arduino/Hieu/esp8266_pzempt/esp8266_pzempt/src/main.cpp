#include <Arduino.h>

#include "water.h"
  
void setup()
{
  Serial.begin(115200);
  delay(10);

  water_sensor_setup();
}
 
void loop()
{
  read_water_loop();
}