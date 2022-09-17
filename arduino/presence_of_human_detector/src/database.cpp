#include "database.h"
#include <EEPROM.h>
#include <stdlib.h>

#define TIME_SYNC_DATABASE          1 /* 100ms */

device_info_t m_device;
uint8_t sync_database_request = 0;

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
    Serial.printf("device_was_calibrated %d\n", m_device.device_was_calibrated);
	Serial.printf("initial_mean %d\n", m_device.initial_mean);
	Serial.printf("range_of_mean %d\n", m_device.range_of_mean);
	Serial.printf("initial_variance %d\n", m_device.initial_variance);
	Serial.printf("range_of_variance %d\n", m_device.range_of_variance);
}

void eeprom_database_loader(void)
{
    memset(&m_device, 0, sizeof(device_info_t));
    EEPROM.get(EEPROM_DB_ADDR, m_device);
    show_database();
}

void sync_database(void)
{
	static uint32_t time_sync_ms = 0;

    if (millis() - time_sync_ms > TIME_SYNC_DATABASE) {
        time_sync_ms = millis();

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

void eeprom_init()
{
    EEPROM.begin(512);
	eeprom_database_loader();
}
