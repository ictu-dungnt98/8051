#include "uno_database.h"
#include <EEPROM.h>
#include "led_button.h"
#include <stdlib.h>
#include "pzem004t.h"


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
    
    EEPROM.get(EEPROM_DB_ADDR, m_device);

    Serial.println("Uno load database:");

    // m_device.time_active_in_week[0] = 0;
    // m_device.time_active_in_week[1] = 0;
    // m_device.time_active_in_week[2] = 0;

    // m_device.time_active_in_month[0] = 0;
    // m_device.time_active_in_month[1] = 0;
    // m_device.time_active_in_month[2] = 0;
}

void eeprom_sync_database(void)
{
    Serial.println("Uno sync database");

    eeprom_clear();
    EEPROM.put(EEPROM_DB_ADDR, m_device);

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

extern float voltage;
extern float current;
extern float power;
extern float energy;
extern float frequency;
extern float pf;

void report_current_state(uint8_t cmd)
{
    char respond[200];
    memset(respond, 0, sizeof(respond));
    char str_vol[5], str_current[5], str_power[5];

    uno_update_current_state_switch();

    voltage = (int)voltage;
    // current = (int)current;
    power = (int)power;

    dtostrf(voltage, 4, 2, str_vol);
    dtostrf(current, 4, 2, str_current);
    dtostrf(power, 4, 2, str_power);



    sprintf(respond, "{\"cmd_type\":%d, \"state\":[%d, %d, %d], \"Voltage\": %s, \"Current\": %s, \"Power\": %s, \"res\":OK}\n",
                            cmd,
                            m_device.m_state_out[0],
                            m_device.m_state_out[1],
                            m_device.m_state_out[2],
                            str_vol, str_current, str_power);

    uno_respond_app(respond);
    Serial.println(respond);
}

/* {cmd_type:4, "dev":0} */
void uno_get_time_active_on_day()
{
    char respond[128];
    uint8_t index;

    for (index = 0; index < NUMBER_CHANNEL; index++) {
        memset(respond, 0, 128);
        sprintf(respond, "{\"cmd_type\":4, \"dev\":%d, \"time_active_day\":[%d, %d], \"res\":OK}\n", \
                                index, \
                                m_device.time_active_on_day[index].tm_hour, \
                                m_device.time_active_on_day[index].tm_min);

        uno_respond_app(respond);
        Serial.println(respond);
    }
}

/* {"cmd_type":5} */
void uno_get_time_active_in_week(void)
{
    char respond[128];
    uint8_t index;

    for (index = 0; index < NUMBER_CHANNEL; index++) {
        memset(respond, 0, 128);
        sprintf(respond, "{\"cmd_type\":5, \"dev\":%d, \"time_active_week\":[%d, %d], \"res\":OK}\n", \
                                index, \
                                m_device.time_active_in_week_tm_hour[index], \
                                m_device.time_active_in_week_tm_min[index]);
        uno_respond_app(respond);
        Serial.println(respond);
    }
}

/* {"cmd_type":6} */
void uno_get_time_active_in_month(void)
{
    char respond[128];
    uint8_t index;

    for (index = 0; index < NUMBER_CHANNEL; index++) {
        memset(respond, 0, 128);
        sprintf(respond, "{\"cmd_type\":6, \"dev\":%d, \"time_active_month\":[%d, %d], \"res\":OK}\n", \
                                index, \
                                m_device.time_active_in_month_tm_hour[index], \
                                m_device.time_active_in_month_tm_min[index]);
        uno_respond_app(respond);
        Serial.println(respond);
    }
}