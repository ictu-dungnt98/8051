#include "dc_motor.h"
#include "lcd_1602.h"

static uint32_t counter = 0;

void countpulse(){
	counter++;

	if (counter == 80){
		stop_dc_motor();
	}
}

void dc_motor_init(void)
{
	pinMode(DC_MOTOR_PIN1, OUTPUT);
	pinMode(DC_MOTOR_PIN2, OUTPUT);

	pinMode(ENCODER_PIN, INPUT_PULLUP);
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

void start_dc_motor(void)
{
	digitalWrite(DC_MOTOR_PIN1, HIGH);
	digitalWrite(DC_MOTOR_PIN2, LOW);
	delay(10);
}

void stop_dc_motor(void)
{
	digitalWrite(DC_MOTOR_PIN1, LOW);
	digitalWrite(DC_MOTOR_PIN2, LOW);
}