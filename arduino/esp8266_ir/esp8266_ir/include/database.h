#ifndef _DB_H_
#define _DB_H_

#include <Arduino.h>
#include <EEPROM.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <string.h>

#include "IRsend.h"

#define EEPROM_DB_ADDR 0

#define NUMBER_KEY 5

typedef struct {
    uint16_t *command_array[NUMBER_KEY];
    uint16_t RawLengthCommand[NUMBER_KEY];
    String name[NUMBER_KEY];
    uint8_t index;
} remote_t;

void eeprom_init(void);
void eeprom_sync_database(void);
void eeprom_database_loader(void);

#endif /* _DB_H_ */