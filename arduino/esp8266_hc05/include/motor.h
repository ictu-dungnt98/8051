#include <Arduino.h>

#define NUM_MOTOR_PIN   4
#define MOTOR1_1        12
#define MOTOR1_2        13
#define MOTOR2_1        14
#define MOTOR2_2        14

void motor_init(void);

void go_left(void);
void go_right(void);
void go_forward(void);
void go_backward(void);
void stop(void);