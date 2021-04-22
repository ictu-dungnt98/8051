
#include "led_button.h"
#include "handler_mqtt.h"
#include "m_typedef.h"

static uint16_t time_button_press[NUMBER_BUTTON] = {0, 0, 0};
static uint8_t m_buttons[NUMBER_BUTTON] = {BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN};
static uint8_t m_leds[NUMBER_LED] = {LED1_PIN, LED2_PIN, LED3_PIN};


void report_current_state()
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":%d, \"state\":[%d, %d, %d], \"res\":1}", CONTROL_IO,
            digitalRead(LED1_PIN), digitalRead(LED2_PIN), digitalRead(LED3_PIN));
    publish_msg(respond);
}

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
    /* toggle value pin */
    digitalWrite(pin, !digitalRead(pin));

    /* Update state and report */
    report_current_state();
}

void control_pin(uint8_t cmd)
{
    switch (cmd)
    {
    case CH1_ON: {
        gpio_on(LED1_PIN);
    } break;
    case CH1_OFF: {
        gpio_off(LED1_PIN);
    } break;

    case CH2_ON: {
        gpio_on(LED2_PIN);
    } break;
    case CH2_OFF: {
        gpio_off(LED2_PIN);
    } break;

    case CH3_ON: {
        gpio_on(LED3_PIN);
    } break;
    case CH3_OFF: {
        gpio_off(LED3_PIN);
    } break;
    
    default: {
    } break;
    }

    /* Update and Report information of this device */
    report_current_state();
}

static void scan_button_handler(uint8_t button_index)
{
   if (!digitalRead(m_buttons[button_index])) /* press */
   {
        time_button_press[button_index] += TIME_SLICE_TO_READ_BUTTON;
   } else { /* realse */
        if (time_button_press[button_index] >= OS_BTN_IS_PRESS_TIME_MIN
            && time_button_press[button_index] <= OS_BTN_IS_PRESS_TIME_MAX)
        {
            if (button_index < 3 ) /* Just have 3 leds */
            {
                /* Control output */
                /* Send message to bluetooth */
                gpio_toggle(m_leds[button_index]);
            }
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

    Serial.println("Button init sucess");
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
        digitalWrite(m_leds[i], HIGH);
    }

    Serial.println("Led init sucess");
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
}

#define TIME_HANDLER_BUTTON         20
static uint32_t time_slice = 0;
static uint32_t time_handler_button_before = 0;
void button_loop(void)
{
    time_slice = millis();

    if (time_slice - time_handler_button_before > TIME_HANDLER_BUTTON) {
        button_handler();
        time_handler_button_before = time_slice;
    }
}