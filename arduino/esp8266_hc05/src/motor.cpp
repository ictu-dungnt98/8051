#include "motor.h"

static uint8_t gs_motors[NUM_MOTOR_PIN] = {MOTOR1_1, MOTOR1_2, MOTOR2_1, MOTOR2_2};

void set_pin(uint8_t pin)
{
    digitalWrite(pin, HIGH);
}

void clear_pin(uint8_t pin)
{
    digitalWrite(pin, LOW);
}

void motor_init(void)
{
    for (int i=0; i < NUM_MOTOR_PIN; i++)
    {
        pinMode(gs_motors[i], OUTPUT);
    }
}

void forward_left(void)
{
	Serial.println("turn left");

	set_pin(MOTOR1_1);
	clear_pin(MOTOR1_2);

	/* forward */
		clear_pin(MOTOR2_1);
	set_pin(MOTOR2_2);
	
	delay(50);
}

void forward_right(void)
{
	Serial.println("turn right");

	clear_pin(MOTOR1_1);
	set_pin(MOTOR1_2);

	/* forward */
	clear_pin(MOTOR2_1);
	set_pin(MOTOR2_2);

	delay(50);
}

void go_forward(void)
{
	Serial.println("turn forward");
	clear_pin(MOTOR2_1);
	set_pin(MOTOR2_2);
	delay(50);
}

void go_backward(void)
{
	Serial.println("turn backward");

	set_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
	delay(50);
}

void back_left(void)
{
	Serial.println("turn left");

	clear_pin(MOTOR1_1);
	set_pin(MOTOR1_2);

	/* go_backward */
	set_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
	
	delay(50);
}

void back_right(void)
{
	Serial.println("turn right");

	set_pin(MOTOR1_1);
	clear_pin(MOTOR1_2);

	/* go_backward */
	set_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);

	delay(50);
}

void stop(void)
{
	Serial.println("turn stop");

	clear_pin(MOTOR1_1);
	clear_pin(MOTOR1_2);
	clear_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
}