#include "dc_motor.h"
#include "lcd_1602.h"

static uint32_t counter = 0;

void countpulse(){
	counter++;
}

void dc_motor_init(void)
{
	pinMode(DC_MOTOR_PIN1, OUTPUT);
	pinMode(DC_MOTOR_PIN2, OUTPUT);

	pinMode(ENCODER_PIN, INPUT);
	attachInterrupt(0,countpulse,FALLING);
}

void dc_motor_loop(void)
{
	static uint32_t time_stamp = 0;
	if (millis() - time_stamp < 100)
		return;
	time_stamp = millis();

	lcd_set_so_vong(counter);
}