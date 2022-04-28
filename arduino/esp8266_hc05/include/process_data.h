#ifndef _process_data_H_
#define _process_data_H_

#include <Arduino.h>
#include <stdio.h>

typedef enum {
	TURN_LEFT = 'L',
	TURN_RIGHT = 'R',
	TURN_FORWARD = 'F',
	TURN_BACKWARD = 'B',
	TURN_STOP = 'S',
	FORWARD_LEFT = 'G',
	FORWARD_RIGHT = 'I',
	BACK_LEFT = 'H',
	BACK_RIGHT = 'J',
	SPEED_10 = 1,
	SPEED_20 = 2,
	SPEED_30 = 3,
	SPEED_40 = 4,
	SPEED_50 = 5,
	SPEED_60 = 6,
	SPEED_70 = 7,
	SPEED_80 = 8,
	SPEED_90 = 9,
	SPEED_100 = 10,
} cmd_t;

typedef struct
{
    char data[256];
    uint32_t len;
} msg_t;

void process_data(char* command);

#endif