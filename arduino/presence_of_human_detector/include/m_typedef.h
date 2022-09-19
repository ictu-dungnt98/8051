#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <Arduino.h>

typedef enum {
    CONTROL_IO = 0,
    QUERY_INFOM = 1,
    SET_ALARM = 2,
    GET_TIME_ALARM = 3,
    RESET_ALARM = 4,
    SET_SENSOR_THRESH_HOLD
} hc06_cmd_type_t;

#endif