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
    uint8_t threshold_type; /* 1 - 2 is over, 3 - 4  is lower for each humi and temper */
    float huminity_threshold;
    float temperature_threshold;
    uint8_t action;
} sensor_t;

typedef struct {
    m_alarm_t m_time_alarm[MAX_CMD_ALARM];
    uint8_t alarm_is_set;
    sensor_t sensor;
} device_info_t;

extern device_info_t m_device;
extern uint8_t sync_database_request;


void eeprom_init(void);
void eeprom_database_loader(void);
void sync_database(void);
void reset_database(void);
#endif






