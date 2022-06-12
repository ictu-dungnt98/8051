#include <Arduino.h>
#include <AccelStepper.h>

#define STEP_PIN 	5 // Step pin
#define DIR_PIN 	6// Dir - pin
#define ENABLE_PIN 	7// Enable pin

void step_motor_init(void);
void step_motor_loop(void);
void set_step_move(uint32_t step_count, uint8_t dir);