#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <Arduino.h>
#include "rtc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define EEPROM_DB_ADDR              0
#define CRC                         0xAABBCCDD

typedef struct {
    char energy[10];
    uint32_t water;
} data_per_month;

typedef struct {
    uint8_t current_month;
    data_per_month data[12]; /* 12 months */
    uint32_t crc_checksum;
} device_info_t;

extern device_info_t m_device;
extern uint8_t sync_database_request;

void database_init(void);
void database_load(void);
void sync_database(void);
void reset_database(void);
#endif






