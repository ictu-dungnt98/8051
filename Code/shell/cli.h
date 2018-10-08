#ifndef _CLI_H_
#define _CLI_H_

#include <REGX51.H>
#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "gpio.h"


#define DELIMITED		"_ "



typedef struct _cli_t_
{
	const char* cmd;
	unsigned int num_parameter;
	const void (*callback)(unsigned char argc,unsigned char** argv);
	struct _cli_t_ *next_cmd;
}cli_t;


extern cli_t* head_cmd;
extern cli_t* current_cmd;


void test_callback(char argc,char** argv);
void help(unsigned char argc, unsigned char** argv);
void cli_init(void);
void cli_add(cli_t* list_cmd, unsigned char num_cmd);
void parse_cli(unsigned char* buffer, unsigned char len_buffer);
char prepare_cli(char rx_data);

#endif