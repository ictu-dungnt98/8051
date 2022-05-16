#include "motor.h"

static uint8_t gs_motors[] = {MOTOR1_1, MOTOR1_2, MOTOR2_1, MOTOR2_2, ENABLE_A, ENABLE_B};

static uint8_t direction = 0;
static int speed_motor_direction = 255;
static int speed_motor_moving = 20;

void set_speed(int speed)
{
	speed_motor_moving = map(speed, 0, 100, 0, 255);
}

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

	speed_motor_moving = map(20, 0, 100, 0, 255);
}

void turn_left(void)
{
	/* direction */
	set_pin(MOTOR1_1);
	clear_pin(MOTOR1_2);
	// analogWrite(ENABLE_A, speed_motor_direction);
}

void turn_right(void)
{
	/* direction */
	clear_pin(MOTOR1_1);
	set_pin(MOTOR1_2);
	// analogWrite(ENABLE_A, speed_motor_direction);
}

void go_forward(void)
{
	Serial.println("turn forward");

	clear_pin(MOTOR2_1);
	set_pin(MOTOR2_2);
	analogWrite(ENABLE_B, speed_motor_moving);
	// delay(100);
}

void go_backward(void)
{
	Serial.println("turn backward");

	set_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
	analogWrite(ENABLE_B, speed_motor_moving);
	// delay(100);
}

void forward_left(void)
{
	Serial.println("turn left");

	if (direction == TURN_LEFT) {
		turn_left();
		/* forward */
		clear_pin(MOTOR2_1);
		set_pin(MOTOR2_2);
		analogWrite(ENABLE_B, speed_motor_moving * 0.5);
		// delay(100);
	} else {
		direction = TURN_LEFT;
		turn_left();
		go_forward();
	}
}

void forward_right(void)
{
	Serial.println("turn right");

	if (direction == TURN_RIGHT) {
		turn_right();
		/* forward */
		clear_pin(MOTOR2_1);
		set_pin(MOTOR2_2);
		analogWrite(ENABLE_B, speed_motor_moving * 0.5);
		// delay(100);
	} else {
		direction = TURN_LEFT;
		turn_right();
		go_forward();
	}
}

void backward_left(void)
{
	Serial.println("turn left");

	if (direction == BACKWARD_LEFT) {
		turn_left();
		/* go_backward */
		set_pin(MOTOR2_1);
		clear_pin(MOTOR2_2);
		analogWrite(ENABLE_B, speed_motor_moving * 0.5);
		// delay(100);
	} else {
		turn_left();
		go_backward();
	}
}

void backward_right(void)
{
	Serial.println("turn right");

	if (direction == BACKWARD_LEFT) {
		turn_right();
		/* go_backward */
		set_pin(MOTOR2_1);
		clear_pin(MOTOR2_2);
		analogWrite(ENABLE_B, speed_motor_moving * 0.5);
		// delay(100);
	} else {
		turn_right();
		go_backward();
	}
}

void stop(void)
{
	Serial.println("turn stop");

	clear_pin(MOTOR1_1);
	clear_pin(MOTOR1_2);
	clear_pin(MOTOR2_1);
	clear_pin(MOTOR2_2);
}