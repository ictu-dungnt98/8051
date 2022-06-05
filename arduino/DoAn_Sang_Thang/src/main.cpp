#include <Arduino.h>
#include "lcd_1602.h"

int in1 = 8; 
int in2 = 9;
int in3 = 10; 
int in4 = 11;  

void setup() 
{
   lcd_init();
   set_so_vong(10);
} 

void loop() 
{
   lcd_loop();
}