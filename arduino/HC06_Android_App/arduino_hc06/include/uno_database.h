#ifndef _UNO_DATABASE_H_
#define _UNO_DATABASE_H_

#include "m_typedef.h"
#include "led_button.h"
#include "rtc.h"

#define EEPROM_DB_ADDR      0

#define time_count_t    uint8_t

typedef struct {
    hc06_ctrl_t m_state_out[NUMBER_CHANNEL];
    uint8_t user_habit[24]; /* 24 hours per day */

    /* Time active */
    struct tm time_active_on_day[NUMBER_CHANNEL];
    uint16_t time_active_in_week[NUMBER_CHANNEL];
    uint16_t time_active_in_month[NUMBER_CHANNEL];

    m_alarm_t m_time_alarm[MAX_CMD_ALARM];
    uint8_t alarm_is_set;

} device_info_t;

void eeprom_database_loader(void);
void eeprom_sync_database(void);

void uno_get_device_infor_jsonform(char *buff, uint16_t len);
void uno_update_current_state_switch(void);
void uno_caculate_time_device_active_loop(void);

void report_current_state(void);

void uno_get_time_active_in_day(void);
void uno_get_time_active_in_week(void);
void uno_get_time_active_in_month(void);

void report_current_state(void);

void uno_get_time_active_on_day(void);
void uno_get_time_active_in_week(void);
void uno_get_time_active_in_month(void);

#endif