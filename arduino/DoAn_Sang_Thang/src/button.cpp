#include <Arduino.h>
#include "button.h"

static uint8_t btn1State = HIGH;
static uint8_t btn2State = HIGH;
static uint8_t btn3State = HIGH;

extern uint8_t enable_motor;
extern void step_motor_reset(void);
void stop_dc_motor(uint8_t is_button_stop);
void start_dc_motor(uint8_t is_button_start);

void button_init(void)
{
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
	pinMode(BUTTON3_PIN, INPUT_PULLUP);
}

void button_loop()
{
	/* start */
    if (digitalRead(BUTTON1_PIN) == LOW) {
        if (btn1State != LOW) {
            Serial.println("BUTTON1_PIN handler");
			start_dc_motor(1);
        }
        btn1State = LOW;
    } else {
        btn1State = HIGH;
    }

	/* stop */
    if (digitalRead(BUTTON2_PIN) == LOW) {
        if (btn2State != LOW) {
            Serial.println("BUTTON2_PIN handler");
			stop_dc_motor(1);
        }
        btn2State = LOW;
    } else {
        btn2State = HIGH;
    }

	/* reset */
	if (digitalRead(BUTTON3_PIN) == LOW) {
        if (btn2State != LOW) {
            Serial.println("BUTTON2_PIN handler");
			step_motor_reset();
        }
        btn3State = LOW;
    } else {
        btn3State = HIGH;
    }
}
