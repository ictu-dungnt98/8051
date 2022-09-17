#include <Arduino.h>

#include "dc_motor.h"
#include "lcd_1602.h"
#include "step_motor.h"
#include "button.h"

void setup()
{
    Serial.begin(115200);

	button_init();
    lcd_init();
    step_motor_init();
    dc_motor_init();
}

void loop()
{
    lcd_loop();
    dc_motor_loop();
	button_loop();
}