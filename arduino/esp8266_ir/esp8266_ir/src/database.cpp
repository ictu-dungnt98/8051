#include "database.h"

extern remote_t my_ir;

void eeprom_init(void) {
    EEPROM.begin(256); /* 256 bytes */
}

/* doc */
void eeprom_database_loader(void)
{
    Serial.println("Uno load database:");
    
    EEPROM.get(EEPROM_DB_ADDR, my_ir);

    Serial.println("Success!!!");
}

static void eeprom_clear()
{
    uint8_t buff[256];

    memset(buff, 0, sizeof(256));

    EEPROM.put(EEPROM_DB_ADDR, buff);

    EEPROM.commit();
}

/* save */
void eeprom_sync_database(void)
{
    Serial.println("Uno sync database");

    /* xoa */
    eeprom_clear();

    /* ghi */
    EEPROM.put(EEPROM_DB_ADDR, my_ir);
    EEPROM.commit();

    Serial.println("Success!!!");
}
