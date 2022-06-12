#include "dc_motor.h"

#include "lcd_1602.h"
#include "step_motor.h"

#define MOTOR_STOP 0
#define MOTOR_RUN 1

extern uint8_t step_motor_state;

static uint32_t counter = 0;
static uint8_t old_dc_motor_state = MOTOR_STOP;
static uint8_t new_dc_motor_state = MOTOR_STOP;

char buffer[16];

void countpulse()
{
    counter++;

    if (counter == 80) {
        stop_dc_motor();
    }
}

void dc_motor_init(void)
{
    pinMode(DC_MOTOR_PIN1, OUTPUT);
    pinMode(DC_MOTOR_PIN2, OUTPUT);

    pinMode(ENCODER_PIN, INPUT_PULLUP);
    attachInterrupt(0, countpulse, FALLING);
}

void dc_motor_loop(void)
{
    static uint32_t time_stamp = 0;
    if (millis() - time_stamp < 100)
        return;
    time_stamp = millis();

    lcd_set_so_vong(counter);

    // if (old_dc_motor_state == new_dc_motor_state)
    //     return;
    // old_dc_motor_state = new_dc_motor_state;

    // if (MOTOR_STOP == new_dc_motor_state) {
    //     Serial.println("MOTOR_STOP");
    //     digitalWrite(DC_MOTOR_PIN1, LOW);
    //     digitalWrite(DC_MOTOR_PIN2, LOW);
	// 	delay(500);
    // } else if (MOTOR_RUN == new_dc_motor_state) {
    //     Serial.println("MOTOR_RUN");
    //     digitalWrite(DC_MOTOR_PIN1, LOW);
    //     digitalWrite(DC_MOTOR_PIN2, HIGH);
    // }
}

void start_dc_motor(void)
{
	Serial.println("start_dc_motor");
    counter = 0;
	digitalWrite(DC_MOTOR_PIN1, LOW);
	digitalWrite(DC_MOTOR_PIN2, HIGH);
}

void stop_dc_motor(void)
{
	Serial.println("stop_dc_motor");
	digitalWrite(DC_MOTOR_PIN1, LOW);
	digitalWrite(DC_MOTOR_PIN2, LOW);
	delay(500);
    set_step_move(1000);
}