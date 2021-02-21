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

typedef enum {
    LEARN_IR_1 = 0,
    LEARN_IR_2 = 1,
    LEARN_IR_3 = 2,
    LEARN_IR_4 = 3,
    LEARN_IR_5 = 4,
    DEVICE_ACTIVE = 5
} state_machine_t;

typedef struct {
    decode_results keyMap[NUMBER_KEY];
    String name_button[NUMBER_KEY];
    uint8_t number_key = 0;
    state_machine_t _state_machine;
} remote_t;

void eeprom_init(void);
void eeprom_sync_database(void);
void eeprom_database_loader(void);

#endif /* _DB_H_ */