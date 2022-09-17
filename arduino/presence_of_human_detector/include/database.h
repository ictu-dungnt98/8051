#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define EEPROM_DB_ADDR              0

typedef struct {
	uint8_t device_was_calibrated;
	int32_t initial_mean;
	int32_t range_of_mean;
	int32_t initial_variance;
	int32_t range_of_variance;
} device_info_t;

extern device_info_t m_device;
extern uint8_t sync_database_request;

void eeprom_init(void);
void eeprom_database_loader(void);
void sync_database(void);
void reset_database(void);

#endif






