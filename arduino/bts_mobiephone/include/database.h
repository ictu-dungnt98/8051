#ifndef _UNO_DATABASE_H_
#define _UNO_DATABASE_H_

#include <Arduino.h>
#include <time.h>

#define EEPROM_DB_ADDR              0
#define NUMBER_CHANNEL				2
#define MAX_CMD_ALARM        6

typedef struct {
    /* Time active */
    struct tm time_active_on_day[NUMBER_CHANNEL];
    uint8_t time_active_in_week_tm_hour[NUMBER_CHANNEL];
    uint8_t time_active_in_week_tm_min[NUMBER_CHANNEL];
    uint8_t time_active_in_month_tm_hour[NUMBER_CHANNEL];
    uint8_t time_active_in_month_tm_min[NUMBER_CHANNEL];
    uint8_t alarm_is_set;
    uint8_t number_days_dev_active; /* count number of day device was active start from first day power up */
} device_info_t;

void eeprom_database_loader(void);
void eeprom_sync_database(void);

void uno_get_device_infor_jsonform(char *buff, uint16_t len);
void uno_update_current_state_switch(void);
void uno_caculate_time_device_active_loop(void);

void report_current_state(uint8_t cmd);
void uno_get_time_alarm_was_set(void);

void uno_get_time_active_in_day(void);
void uno_get_time_active_in_week(void);
void uno_get_time_active_in_month(void);

void uno_get_time_active_on_day(void);
void uno_get_time_active_in_week(void);
void uno_get_time_active_in_month(void);

#endif