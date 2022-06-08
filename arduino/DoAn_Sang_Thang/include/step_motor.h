#include <Arduino.h>
#include <AccelStepper.h>

#define STEP_PIN 	5 // Step pin
#define DIR_PIN 	6// Dir - pin
#define ENABLE_PIN 	7// Enable pin

void step_motor_init(void);
void motor_loop(void);