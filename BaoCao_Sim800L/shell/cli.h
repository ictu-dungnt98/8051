#ifndef _CLI_H_
#define _CLI_H_

#include <REGX51.H>
#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "timer.h"

#define DELIMITED		"_ \n\r"

char prepare_cli(char rx_data);
void parse_cli(unsigned char* buffer, unsigned char len_buffer);

#endif