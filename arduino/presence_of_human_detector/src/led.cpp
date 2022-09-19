
#include "led.h"
#include "m_typedef.h"

static uint8_t m_leds[NUMBER_LED] = {LED1_PIN};

void gpio_on(uint8_t pin) { digitalWrite(pin, HIGH); }

void gpio_off(uint8_t pin) { digitalWrite(pin, LOW); }

void gpio_toggle(uint8_t pin)
{
    /* toggle value pin */
    digitalWrite(pin, !digitalRead(pin));
}

void led_init(void)
{
    int i;

    for (i = 0; i < NUMBER_LED; i++) {
        pinMode(m_leds[i], OUTPUT);
    }

    /* Turn off all */
    for (i = 0; i < NUMBER_LED; i++) {
        digitalWrite(m_leds[i], LOW);
    }

    Serial.println("Led init sucess");
}
