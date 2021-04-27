#include <Arduino.h>
#include "pzem004t.h"
#include "water.h"
  
void setup()
{
  Serial.begin(115200);
  delay(10);

  water_sensor_setup();
  pzem_init();
}
 
void loop()
{
  read_water_loop();
  pzem004t_loop();
}
