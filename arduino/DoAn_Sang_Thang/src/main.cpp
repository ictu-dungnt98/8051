#include <Arduino.h>
#include "lcd_1602.h"
#include "step_motor.h"
#include "dc_motor.h"

void setup() 
{
   lcd_init();
   step_motor_init();
   dc_motor_init();
} 

void loop() 
{
   lcd_loop();
   motor_loop();
   dc_motor_loop();
}