#include <Arduino.h>

#define ENCODER_PIN 2
#define DC_MOTOR_PIN1 3
#define DC_MOTOR_PIN2 4

void dc_motor_init(void);
void dc_motor_loop(void);
void start_dc_motor(uint8_t is_button_start);
void stop_dc_motor(uint8_t is_button_stop);