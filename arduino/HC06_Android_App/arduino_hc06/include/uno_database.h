#ifndef _UNO_DATABASE_H_
#define _UNO_DATABASE_H_

#include "m_typedef.h"
#include "led_button.h"

#define time_count_t    uint8_t

typedef struct {
    hc06_ctrl_t m_state_out[NUMBER_CHANNEL];

    /* Time active */
    struct tm time_active_on_day[NUMBER_CHANNEL];
    uint16_t time_active_in_week[NUMBER_CHANNEL];
    uint16_t time_active_in_month[NUMBER_CHANNEL];

} device_info_t;

void uno_get_device_infor_jsonform(char *buff, uint16_t len);
void uno_update_current_state_switch(void);
void uno_caculate_time_device_active_loop(void);

void report_current_state(void);

void uno_get_time_active_in_day();
void uno_get_time_active_in_week();
void uno_get_time_active_in_month();

void report_current_state(void);

void uno_get_time_active_on_day(uint8_t index);
void uno_get_time_active_in_week();
void uno_get_time_active_in_month();

#endif