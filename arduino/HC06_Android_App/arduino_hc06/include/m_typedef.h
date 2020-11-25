#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    CH1_ON  = 1,
    CH1_OFF = 2,
    CH2_ON  = 3,
    CH2_OFF = 4,
    CH3_ON  = 5,
    CH3_OFF = 6,
} hc06_ctrl_t;

typedef enum {
    CONTROL_IO = 0,
    SET_ALARM = 1,
    QUERY_INFOM = 2
} hc06_cmd_type_t;

#endif