#include <Arduino.h>
#include <stdint.h>
#include "hardware.h"
#include "app_btn.h"

static const char *event_name[] = {"pressed", "release",
                                   "hold", "hold so long",
                                   "double click", "tripple click",
                                   "idle", "idle break", "invalid"};
void on_button_event_cb(int button_pin, int event, void *data)
{
    Serial.printf("Button %d, event %s\r\n", button_pin, event_name[event]);
    
    switch (event)
    {
    case APP_BTN_EVT_PRESSED:
    {
    }
    break;

    case APP_BTN_EVT_RELEASED:
    {
    }
    break;

    case APP_BTN_EVT_HOLD:
    {
    }
    break;

    case APP_BTN_EVT_HOLD_SO_LONG:
    {
    }
    break;

    case APP_BTN_EVT_DOUBLE_CLICK:
    {
    }
    break;

    case APP_BTN_EVT_TRIPLE_CLICK:
    {
    }
    break;

    default:
        break;
    }
}

uint32_t btn_read(uint32_t index)
{
	int adc_value = analogRead(index);
	float voltage = adc_value * (1.0 / 1023.0);
	Serial.printf("adc_value: %d, voltage: %f\r\n", adc_value, voltage);

	return 1;
}

void m_button_init(void)
{
    static app_btn_hw_config_t m_hw_button_initialize_value[APP_BTN_MAX_BTN_SUPPORT];
    
    for (uint32_t i = 0; i < APP_BTN_MAX_BTN_SUPPORT; i++)
    {
        m_hw_button_initialize_value[i].idle_level = 1;
        m_hw_button_initialize_value[i].last_state = btn_read(i); // read the very first level at initialization
        m_hw_button_initialize_value[i].pin = i;
    }

    app_btn_config_t conf;
    conf.config = &m_hw_button_initialize_value[0];
    conf.btn_count = APP_BTN_MAX_BTN_SUPPORT;
    conf.get_tick_cb = sys_get_tick_ms;
    conf.btn_initialize = NULL;
    conf.btn_read = btn_read;

    app_btn_initialize(&conf);
    app_btn_register_callback(APP_BTN_EVT_PRESSED, on_button_event_cb, NULL);
    app_btn_register_callback(APP_BTN_EVT_RELEASED, on_button_event_cb, NULL);
    app_btn_register_callback(APP_BTN_EVT_HOLD, on_button_event_cb, NULL);
    app_btn_register_callback(APP_BTN_EVT_HOLD_SO_LONG, on_button_event_cb, NULL);
}

/* private */
void button_scan_task(void *param)
{
	static uint32_t old_tick = 0;

	if (sys_get_tick_ms() - old_tick > 50) {
		old_tick = sys_get_tick_ms();
		app_btn_scan(NULL);
	}
}