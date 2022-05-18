#include "database.h"
#include <EEPROM.h>
#include <stdlib.h>



device_info_t m_device;

uint8_t uno_sync_database_request = 0;

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
}

void eeprom_sync_database(void)
{
    eeprom_clear();
    EEPROM.put(EEPROM_DB_ADDR, m_device);

    uno_sync_database_request = 0;
}
