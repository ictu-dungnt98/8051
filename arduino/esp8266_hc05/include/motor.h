#include <Arduino.h>

#define NUM_MOTOR_PIN   4
// direction
#define MOTOR1_1        14
#define MOTOR1_2        12
// up down
#define MOTOR2_1        4
#define MOTOR2_2        5

void motor_init(void);

void forward_left(void);
void forward_right(void);
void backward_left(void);
void backward_right(void);

void go_forward(void);
void go_backward(void);

void stop(void);