#include "uno_database.h"
#include <EEPROM.h>
#include "led_button.h"
#include <stdlib.h>

device_info_t m_device;
m_user_habit_trace_t user_habit[NUMBER_CHANNEL];

uint8_t uno_sync_database_request = 0;

static void eeprom_clear()
{
    for (uint8_t i = 0 ; i < (sizeof(device_info_t)+1); i++) {
        EEPROM.write(i, 0);
    }
}

void eeprom_database_loader(void)
{
    memset(&user_habit, 0, sizeof(user_habit));
    memset(&m_device, 0, sizeof(device_info_t));
    // uno_get_time_active_on_day(0);

    EEPROM.get(EEPROM_DB_ADDR, m_device);

    uno_get_time_active_on_day(0);
}

void eeprom_sync_database(void)
{
    // uno_get_time_active_on_day(0);

    Serial.println("Uno sync database");

    eeprom_clear();
    EEPROM.put(EEPROM_DB_ADDR, m_device);

    // EEPROM.get(EEPROM_DB_ADDR, m_device);
    uno_get_time_active_on_day(0);

    uno_sync_database_request = 0;
}

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
    sprintf(respond, "{\"cmd_type\":4, \"dev\":%d, \"time_active_day\":[%d, %d, %d]}\n", \
                            index, \
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