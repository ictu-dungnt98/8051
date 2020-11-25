#include "led_button.h"

void gpio_on(uint8_t pin)
{
    digitalWrite(pin, HIGH);
}

void gpio_off(uint8_t pin)
{
    digitalWrite(pin, LOW);
}

void gpio_toggle(uint8_t pin)
{
    digitalWrite(pin, !digitalRead(pin));
}

static uint16_t time_button_press[NUMBER_BUTTON] = {0, 0, 0, 0};
static uint8_t m_buttons[NUMBER_BUTTON] = {BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN};
static uint8_t m_leds[NUMBER_LED] = {LED1_PIN, LED2_PIN, LED3_PIN};

static void scan_button_handler(uint8_t button_index)
{
   if (!digitalRead(m_buttons[button_index])) /* press */
   {
        time_button_press[button_index] += TIME_SLICE_TO_READ_BUTTON;
   } else { /* realse */
        if (time_button_press[button_index] >= OS_BTN_IS_PRESS_TIME_MIN
            && time_button_press[button_index] <= OS_BTN_IS_PRESS_TIME_MAX)
        {
            Serial.println("Button was press");
            if (button_index < 4 ) /* Just have 3 leds */ {
                /* Control output */
                gpio_toggle(m_leds[button_index]);

                /* Send message to bluetooth */
            } else { /* handler button remove alarm */
                Serial.println("Button was hold");
            }
        }
        else /* BUTTON is Hold */
        {
            /* handler event button was hold long time */
        }

        time_button_press[button_index] = 0;
   }
}

static void button_init(void)
{
    int i;

    for(i=0; i < NUMBER_BUTTON; i++)
    {
        pinMode(m_buttons[i], INPUT_PULLUP);
    }
}

static void led_init(void)
{
    int i;

    for (i=0; i < NUMBER_LED; i++)
    {
        pinMode(m_leds[i], OUTPUT);
    }

    /* Turn off all */
    for (i=0; i < NUMBER_LED; i++)
    {
        digitalWrite(m_leds[i], LOW);
    }
}

void led_button_init()
{
    button_init();
    led_init();
}

void button_handler(void)
{
    scan_button_handler(BUTTON1);
    scan_button_handler(BUTTON2);
    scan_button_handler(BUTTON3);
    scan_button_handler(BUTTON4);
}