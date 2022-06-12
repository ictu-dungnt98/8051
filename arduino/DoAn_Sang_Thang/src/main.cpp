#include <Arduino.h>
#include "lcd_1602.h"
#include "step_motor.h"
#include "dc_motor.h"

static uint32_t step_count = 0;

void setup() 
{
   lcd_init();
   step_motor_init();
   dc_motor_init();
   start_dc_motor();
} 

void loop() 
{
   lcd_loop();
   dc_motor_loop();
   
   step_count += 2;
   set_step_move(step_count);
//    step_motor_loop();
}