#ifndef _process_data_H_
#define _process_data_H_

#include <Arduino.h>
#include <stdio.h>

typedef struct
{
    char data[256];
    uint32_t len;
} msg_t;

void process_data(char* command);
void process_data_by_byte(char cmd_type);

#endif