#include "dc_motor.h"

#include "lcd_1602.h"
#include "step_motor.h"

#define MOTOR_STOP 0
#define MOTOR_RUN 1
#define SO_VONG_DAO_CHIEU 23

extern uint8_t step_motor_state;

static uint32_t counter = 0;
static uint32_t number_circle = 0;
static uint32_t dir = 0;
static uint8_t old_dc_motor_state = MOTOR_STOP;
static uint8_t new_dc_motor_state = MOTOR_STOP;

uint8_t enable_motor = 0;

void countpulse()
{
    counter++;

    if (counter == 60) {
		number_circle ++;
        stop_dc_motor(0);
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

    lcd_set_so_vong(number_circle);

    if (old_dc_motor_state == new_dc_motor_state)
        return;
    old_dc_motor_state = new_dc_motor_state;

    if (MOTOR_STOP == new_dc_motor_state) {
        Serial.println("MOTOR_STOP");
		if (enable_motor == 0)
			return;

		/* sua so nay 23 */
		if (number_circle % SO_VONG_DAO_CHIEU == 0 && number_circle != 0) {
			dir = !dir;
		}
		set_step_move(1000, dir);
    } else if (MOTOR_RUN == new_dc_motor_state) {
        Serial.println("MOTOR_RUN");
    }
}

void start_dc_motor(uint8_t is_button_start)
{
	Serial.println("start_dc_motor");
    new_dc_motor_state = MOTOR_RUN;
	enable_motor = 1;

	if (is_button_start == 0)
	    counter = 0;

	digitalWrite(DC_MOTOR_PIN1, LOW);
	digitalWrite(DC_MOTOR_PIN2, HIGH);
}

void stop_dc_motor(uint8_t is_button_stop)
{
	Serial.println("stop_dc_motor");
    new_dc_motor_state = MOTOR_STOP;

	if (is_button_stop)
		enable_motor = 0;

	digitalWrite(DC_MOTOR_PIN1, LOW);
	digitalWrite(DC_MOTOR_PIN2, LOW);
}

void step_motor_reset(void)
{
	for (uint32_t i = 0; i < number_circle % SO_VONG_DAO_CHIEU; i++) {
		step_motor_back(1000);
	}

	enable_motor = 0;
	new_dc_motor_state = MOTOR_STOP;
	old_dc_motor_state = MOTOR_STOP;
	number_circle = 0;
	counter = 0;

	stop_dc_motor(0);
}