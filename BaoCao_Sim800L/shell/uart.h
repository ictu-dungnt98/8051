#ifndef _UART_H_
#define _UART_H_

#include <REGX51.H>


#define BUFFER_SIZE		50

extern volatile char _data;
extern char buffer[BUFFER_SIZE];
extern unsigned char write_index;


void uart_init(void);
void uart_send_byte(char data_);
void uart_send_str(char* str);

#endif