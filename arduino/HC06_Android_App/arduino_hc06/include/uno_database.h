#ifndef _UNO_DATABASE_H_
#define _UNO_DATABASE_H_

#include "m_typedef.h"

#define time_count_t    uint8_t

typedef struct {
    hc06_ctrl_t m_state_out1;
    hc06_ctrl_t m_state_out2;
    hc06_ctrl_t m_state_out3;

    /* Time active */
    time_count_t time_on_out1;
    time_count_t time_off_out1;
    time_count_t time_on_out2;
    time_count_t time_off_out2;
    time_count_t time_on_out3;
    time_count_t time_off_out3;
} device_info_t;

void uno_get_device_infor_jsonform(char *buff, uint16_t len);
void os_update_current_state_switch(hc06_ctrl_t state);

#endif