#include "uno_database.h"
#include "led_button.h"

device_info_t m_device;

void uno_update_current_state_switch(void)
{
    if (digitalRead(LED1_PIN)) {
        m_device.m_state_out[0] = CH1_ON;
    } else {
        m_device.m_state_out[0] = CH1_OFF;
    }

    if (digitalRead(LED2_PIN)) {
        m_device.m_state_out[1] = CH2_ON;
    } else {
        m_device.m_state_out[1] = CH2_OFF;
    }

    if (digitalRead(LED3_PIN)) {
        m_device.m_state_out[2] = CH3_ON;
    } else {
        m_device.m_state_out[2] = CH3_OFF;
    }
}




void report_current_state(void)
{
    char respond[128];
    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":0, \"state\":[%d, %d, %d]}\n",
                            m_device.m_state_out[0],
                            m_device.m_state_out[1],
                            m_device.m_state_out[2]);
    uno_respond_app(respond);
    Serial.println(respond);
}

/* {cmd_type:4, "dev":0} */
void uno_get_time_active_on_day(uint8_t index)
{
    char respond[128];
    memset(respond, 0, 128);
    sprintf(respond, "{\"cmd_type\":4, \"time_active_day\":[%d, %d, %d]}\n", \
                            m_device.time_active_on_day[index].tm_hour, \
                            m_device.time_active_on_day[index].tm_min, \
                            m_device.time_active_on_day[index].tm_sec);

    uno_respond_app(respond);
    Serial.println(respond);
}

/* {"cmd_type":5} */
void uno_get_time_active_in_week(void)
{
    char respond[128];
    memset(respond, 0, 128);
    sprintf(respond, "{\"cmd_type\":5, \"time_active_week\":[%d, %d, %d]}\n", \
                            m_device.time_active_in_week[0],
                            m_device.time_active_in_week[1],
                            m_device.time_active_in_week[2]);

    uno_respond_app(respond);
    Serial.println(respond);
}

/* {"cmd_type":6} */
void uno_get_time_active_in_month(void)
{
    char respond[128];
    memset(respond, 0, 128);
    sprintf(respond, "{\"cmd_type\":6, \"time_active_month\":[%d, %d, %d]}\n", \
                            m_device.time_active_in_month[0],
                            m_device.time_active_in_month[1],
                            m_device.time_active_in_month[2]);

    uno_respond_app(respond);
    Serial.println(respond);
}