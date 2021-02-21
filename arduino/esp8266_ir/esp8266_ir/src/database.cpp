#include "database.h"

extern state_machine_t state_machine;
extern remote_t m_remote;

void eeprom_init(void) {
    EEPROM.begin(sizeof(remote_t) + 1);
}

void eeprom_database_loader(void)
{
    Serial.println("Uno load database:");
    
    memset(&m_remote, 0, sizeof(m_remote));
    EEPROM.get(EEPROM_DB_ADDR, m_remote);

    Serial.print("number_key: ");
    Serial.println(m_remote.number_key);
    Serial.print("keyMap: ");
    serialPrintUint64(m_remote.keyMap[m_remote.number_key-1].value, HEX);
    Serial.println();
    Serial.print("name_button: ");
    Serial.println(m_remote.name_button[m_remote.number_key-1]);
    Serial.print("_state_machine: ");
    Serial.println(m_remote._state_machine);

    state_machine = m_remote._state_machine;

    Serial.println("Success!!!");
}

static void eeprom_clear()
{
    remote_t tmp_remote;
    memset(&tmp_remote, 0, sizeof(tmp_remote));
    EEPROM.put(EEPROM_DB_ADDR, tmp_remote);
    // for (uint32_t i = 0 ; i < (sizeof(remote_t)+1); i++) {
    //     EEPROM.write(i, 0);
    //     Serial.println(i);
    // }
    EEPROM.commit();
}
void eeprom_sync_database(void)
{
    Serial.println("Uno sync database");

    eeprom_clear();

    Serial.print("number_key: ");
    Serial.println(m_remote.number_key);
    Serial.print("keyMap: ");
    serialPrintUint64(m_remote.keyMap[m_remote.number_key-1].value, HEX);
    Serial.println();
    Serial.print("name_button: ");
    Serial.println(m_remote.name_button[m_remote.number_key-1]);
    Serial.print("_state_machine: ");
    Serial.println(m_remote._state_machine);

    EEPROM.put(EEPROM_DB_ADDR, m_remote);
    EEPROM.commit();

    Serial.println("Success!!!");
}
