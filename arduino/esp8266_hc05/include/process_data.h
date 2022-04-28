#ifndef _process_data_H_
#define _process_data_H_

#include <Arduino.h>
#include <stdio.h>

typedef enum {
	TURN_LEFT = 1,
	TURN_RIGHT,
	TURN_FORWARD,
	TURN_BACKWARD,
	TURN_STOP,
} cmd_t;

typedef struct
{
    char data[256];
    uint32_t len;
} msg_t;

void process_data(char* command);

#endif