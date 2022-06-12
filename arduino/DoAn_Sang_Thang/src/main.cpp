#include <Arduino.h>

#include "dc_motor.h"
#include "lcd_1602.h"
#include "step_motor.h"

static uint32_t step_count = 0;

void setup()
{
    Serial.begin(115200);

    lcd_init();
    step_motor_init();
    dc_motor_init();
    start_dc_motor();
}

void loop()
{
    lcd_loop();
    dc_motor_loop();
    step_motor_loop();
}