#include "uart.h"

volatile char _data = 0;
char buffer[BUFFER_SIZE] = "";
unsigned char write_index = 0;
unsigned char read_index = 0;

/****************** UART ******************************/
void uart_init()
{
	TMOD = 0x20;	 // timer1 mode 2 auto update
	SCON = 0x50;	// uart 8bit baudrate is selected by timer1
	TH1 = 0xFD; //9600	
	IE = 0x90;    // enable interrupt uart 
	TR1 = 1;
}
/* UART send 1 byte */
void uart_send_byte(char data_)
{
	SBUF = data_;
	while(TI ==0){}
	TI = 0;

}
/* UART send string */
void uart_send_str(char* str)
{
	while(*str)
	{
		uart_send_byte(*str);
		str++;	
	}
}
