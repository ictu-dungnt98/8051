#include "database.h"
#include "led_button.h"
#include "handler_mqtt.h"

#include <EEPROM.h>
#include <stdlib.h>


device_info_t m_device;
uint8_t sync_database_request = 0;

void eeprom_init()
{
    EEPROM.begin(512);
}

void eeprom_clear()
{
    for (uint16_t i = 0 ; i < (sizeof(device_info_t)+1); i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

void reset_database()
{
    Serial.println("reset_database");
    eeprom_clear();
    memset(&m_device, 0, sizeof(device_info_t));
}

void show_database()
{
    Serial.printf("alarm_is_set %d\n", m_device.alarm_is_set);

    uint8_t index;
    for (index = 0; index < MAX_CMD_ALARM; index++) {
        if (m_device.m_time_alarm[index].m_cmd != 0) {
            Serial.printf("alarm_is_set %d\n", m_device.m_time_alarm[index].m_cmd);
            Serial.printf("tm_hour %d\n", m_device.m_time_alarm[index].m_time.tm_hour);
            Serial.printf("tm_min %d\n", m_device.m_time_alarm[index].m_time.tm_min);
        }
    }
}

void eeprom_database_loader(void)
{
    memset(&m_device, 0, sizeof(device_info_t));
    
    EEPROM.get(EEPROM_DB_ADDR, m_device);
    show_database();
}

#define TIME_SYNC_DATABASE          1 /* 100ms */
static uint32_t time_handler_sync_db_before = 0;

void sync_database(void)
{
    if (millis() - time_handler_sync_db_before > TIME_SYNC_DATABASE) {
        time_handler_sync_db_before = millis();

        if (sync_database_request != 0) {
 
            Serial.println("sync_database");


            eeprom_clear();
            EEPROM.put(EEPROM_DB_ADDR, m_device);
            EEPROM.commit();
            show_database();

            sync_database_request = 0;
        }
    }
}
