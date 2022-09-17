#include "step_motor.h"

extern void start_dc_motor(uint8_t is_button_start);

/* Hardware driver: A4988 */
AccelStepper stepper(1, 5, 4);  // pin 5 step, pin 4 dir

static uint32_t _step_count = 0;
uint8_t step_motor_state = 0;

void set_step_move(uint32_t step_count, uint8_t dir)
{
    step_motor_state = 1;

	digitalWrite(DIR_PIN, dir);

    _step_count += step_count;
    stepper.moveTo(_step_count);
    stepper.setMaxSpeed(1000000);      // chỉnh tốc độ.
    stepper.setAcceleration(1000000);  // chỉnh gia tốc.

    while (1) {
        if (stepper.distanceToGo() == 0) {
            step_motor_state = 0;
			break;
        }
        stepper.run();
    }
	delay(200);
	start_dc_motor(0);
}

void step_motor_init()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);  // Enable
    digitalWrite(ENABLE_PIN, LOW);  // Dặt Enable xuống low để khởi động động cơ
}

void step_motor_back(uint32_t step_count)
{
	digitalWrite(DIR_PIN, 1);

    _step_count += step_count;
    stepper.moveTo(_step_count);
    stepper.setMaxSpeed(1000000);      // chỉnh tốc độ.
    stepper.setAcceleration(1000000);  // chỉnh gia tốc.

    while (1) {
        if (stepper.distanceToGo() == 0) {
			break;
        }
        stepper.run();
    }
}