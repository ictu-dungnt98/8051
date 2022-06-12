#include "step_motor.h"

/* Hardware driver: A4988 */
AccelStepper stepper(1, 5, 4);  // pin 5 step, pin 4 dir

static uint32_t step_count = 0;

void set_step_move(uint32_t step_count)
{
	stepper.setMaxSpeed(100);
	stepper.setSpeed(100);
	stepper.moveTo(step_count);
}

void step_motor_init()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);  // Enable
    digitalWrite(ENABLE_PIN, LOW);  // Dặt Enable xuống low để khởi động động cơ
}

void motor_loop(void)
{
	static uint32_t time_stamp = 0;
	if (millis() - time_stamp < 5)
		return;
	time_stamp = millis();
	
	// step_count ++;
	
    if (stepper.distanceToGo() == 0) {
        stepper.moveTo(step_count);
        stepper.setMaxSpeed(500);      // chỉnh tốc độ.
        stepper.setAcceleration(500);  // chỉnh gia tốc.
    }
    stepper.run();
}