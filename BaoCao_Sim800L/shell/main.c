#include <REGX51.H>
#include "./uart.h"
#include "./cli.h"
unsigned char i = 0;
void main()
{
	P0 = 0x00;
	P1 = 0x00;
	P2 = 0xff;
	uart_init();
	timer0_init();
	write_index = 0;
	memset(buffer,0,BUFFER_SIZE);
	_data = 0;

	delay(15000);	

	do{
    	uart_send_str("ATD+84982634613;\r\n");
  	}while(_data !=0);


	_data = 0;

	while(1)
	{	
		if(_data != 0)
		{			
			prepare_cli(_data);
			_data = 0;
		}	
	}				
}
/****************** INTERRUPT  ******************************/
void timer0_ISR(void) interrupt 1
{
	 tick++;
	 TH0 = 0xFC;
	 TL0 = 0x66;
}
/* UART receive data interrupt */
void uart_ISR(void) interrupt 4
{
	if(RI == 1)
	{
		_data = SBUF;
		RI = 0;
	}
}