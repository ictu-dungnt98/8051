#include "database.h"
#include "handler_mqtt.h"

#include <EEPROM.h>
#include <stdlib.h>


device_info_t m_device;
uint8_t sync_database_request = 0;

void database_init()
{
    EEPROM.begin(512);
}

void eeprom_clear()
{
    for (uint8_t i = 0 ; i < (sizeof(device_info_t)+1); i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

void reset_database()
{
    // Serial.println("reset_database");
    eeprom_clear();
    memset(&m_device, 0, sizeof(device_info_t));
}

void show_database()
{
    return;
}

void database_load(void)
{
    memset(&m_device, 0, sizeof(device_info_t));
    
    EEPROM.get(EEPROM_DB_ADDR, m_device);

    if (m_device.crc_checksum != CRC) {
         memset(&m_device, 0, sizeof(device_info_t));
         m_device.crc_checksum = CRC;
    }
    
    show_database();
}

#define TIME_SYNC_DATABASE          10 /* 100ms */
static uint32_t time_handler_sync_db_before = 0;

void sync_database(void)
{
    if (millis() - time_handler_sync_db_before > TIME_SYNC_DATABASE) {
        time_handler_sync_db_before = millis();

        if (sync_database_request != 0) {
 
            // Serial.println("sync_database");

            eeprom_clear();
            EEPROM.put(EEPROM_DB_ADDR, m_device);
            EEPROM.commit();
            show_database();

            sync_database_request = 0;
        }
    }
}
