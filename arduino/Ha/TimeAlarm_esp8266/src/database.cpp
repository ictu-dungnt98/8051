#include "database.h"
#include "led_button.h"
#include "handler_mqtt.h"

#include <EEPROM.h>
#include <stdlib.h>


device_info_t m_device;
uint8_t sync_database_request = 0;

static void eeprom_clear()
{
    for (uint8_t i = 0 ; i < (sizeof(device_info_t)+1); i++) {
        EEPROM.write(i, 0);
    }
}

void eeprom_database_loader(void)
{
    memset(&m_device, 0, sizeof(device_info_t));
    
    EEPROM.get(EEPROM_DB_ADDR, m_device);

    Serial.println("Uno load database:");
}

#define TIME_SYNC_DATABASE          20000 /* 10s */
static uint32_t time_handler_sync_db_before = 0;

void sync_database(void)
{
    if (millis() - time_handler_sync_db_before > TIME_SYNC_DATABASE) {
        if (sync_database_request != 0) {
            eeprom_clear();
            EEPROM.put(EEPROM_DB_ADDR, m_device);
            
            sync_database_request = 0;
        }

        time_handler_sync_db_before = millis();
    }
}
