#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <Arduino.h>
#include "m_typedef.h"
#include "led_button.h"
#include "rtc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define EEPROM_DB_ADDR              0

typedef struct {
    struct tm m_time;
    uint8_t m_cmd;
} m_alarm_t;

typedef struct {
    m_alarm_t m_time_alarm[MAX_CMD_ALARM];
    uint8_t alarm_is_set;
} device_info_t;

extern device_info_t m_device;
extern uint8_t sync_database_request;

void eeprom_database_loader(void);
void sync_database(void);
#endif






