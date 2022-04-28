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

void go_left(void)
{
	Serial.println("turn left");

	set_pin(MOTOR1_1);
	clear_pin(MOTOR1_2);
}

void go_right(void)
{
	Serial.println("turn right");

	clear_pin(MOTOR1_1);
	set_pin(MOTOR1_2);
}

void go_forward(void)
{
	Serial.println("turn forward");

	set_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
}

void go_backward(void)
{
	Serial.println("turn backward");

	clear_pin(MOTOR2_1);
	set_pin(MOTOR2_2);
}

void stop(void)
{
	Serial.println("turn stop");

	clear_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
}