#include <Arduino.h>
#include "button.h"

static uint8_t btn1State = HIGH;
static uint8_t btn2State = HIGH;

void button_init(void)
{
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void button_loop()
{
    if (digitalRead(BUTTON1_PIN) == LOW) {
        // btnState is used to avoid sequential toggles
        if (btn1State != LOW) {
            Serial.println("BUTTON1_PIN handler");
        }
        btn1State = LOW;
    } else {
        btn1State = HIGH;
    }

    if (digitalRead(BUTTON2_PIN) == LOW) {
        // btnState is used to avoid sequential toggles
        if (btn2State != LOW) {
            Serial.println("BUTTON2_PIN handler");
        }
        btn2State = LOW;
    } else {
        btn2State = HIGH;
    }
}
