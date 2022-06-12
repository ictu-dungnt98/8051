#include "step_motor.h"

extern void start_dc_motor(void);
extern void stop_dc_motor(void);

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
	start_dc_motor();
}

void step_motor_init()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);  // Enable
    digitalWrite(ENABLE_PIN, LOW);  // Dặt Enable xuống low để khởi động động cơ
}

void step_motor_loop(void)
{
    // static uint32_t time_stamp = 0;
    // if (millis() - time_stamp < 5)
    //     return;
    // time_stamp = millis();

    // if (step_motor_state == 0)
    //     return;

    // if (stepper.distanceToGo() == 0) {
    //     start_dc_motor();
    //     step_motor_state = 0;
    // }
    // stepper.run();
}