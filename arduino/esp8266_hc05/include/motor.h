#include <Arduino.h>

#define NUM_MOTOR_PIN   6
// direction
#define MOTOR1_1        14
#define MOTOR1_2        12
#define ENABLE_A		16
// up down
#define MOTOR2_1        4
#define MOTOR2_2        5
#define ENABLE_B		15

typedef enum {
	TURN_LEFT = 'L',
	TURN_RIGHT = 'R',
	TURN_FORWARD = 'F',
	TURN_BACKWARD = 'B',
	TURN_STOP = 'S',
	FORWARD_LEFT = 'G',
	FORWARD_RIGHT = 'I',
	BACKWARD_LEFT = 'H',
	BACKWARD_RIGHT = 'J',
	SPEED_10 = 1,
	SPEED_20 = 2,
	SPEED_30 = 3,
	SPEED_40 = 4,
	SPEED_50 = 5,
	SPEED_60 = 6,
	SPEED_70 = 7,
	SPEED_80 = 8,
	SPEED_90 = 9,
	SPEED_100 = 10,
} cmd_t;

void motor_init(void);
void set_speed(int speed);

void turn_left(void);
void turn_right(void);

void forward_left(void);
void forward_right(void);
void backward_left(void);
void backward_right(void);

void go_forward(void);
void go_backward(void);


void stop(void);