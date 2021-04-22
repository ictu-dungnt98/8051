#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <Arduino.h>

#define HC06    Serial1

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
    QUERY_INFOM = 1,
    SET_ALARM = 2,
    GET_TIME_ALARM = 3,
    RESET_ALARM = 4,
} hc06_cmd_type_t;


#define os_trace    Serial.println

#endif