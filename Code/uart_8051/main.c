#include <REGX51.H>
#include <stdlib.h>


#define BUFFER_SIZE		2

volatile unsigned long tick = 0;
volatile char _data = '\0';

char buffer[BUFFER_SIZE] = "";
unsigned char write_index = 0;
unsigned char read_index = 0;

/****************** delay function ******************************/
void delay(unsigned long time)
{
	unsigned long current_tick = tick;
	while(!(tick - current_tick == time));
}

/****************** TIMER ******************************/
void timer0_init()	   // 1ms
{
	TMOD &= ~0x0F;
	TMOD |= 0x01; // 16 bit

	TH0 = 0xFC;
	TL0 = 0x67;

	IE = 0x82;   // enable timer0 interrupt
	
	TR0 = 1;
}


/****************** UART ******************************/
void uart_init()
{
	TMOD = 0x20;	 // timer1 mode 2 auto update
	SCON = 0x50;	// uart 8bit baudrate is selected by timer1
	TH1 = 0xFD; //9600	
	IE = 0x90;    // enable interrupt uart 
	TR1 = 1;
}

void uart_send_byte(char data_)
{
	SBUF = data_;
	while(TI ==0){}
	TI = 0;

}

void uart_send_str(char* str)
{
	while(*str)
	{
		uart_send_byte(*str);
		str++;	
	}
}
	int result = 0;
	int port,pin;
	char val = 0;
/********************************************************************************/
/****************************    MAIN   ****************************************/
/******************************************************************************/
void main()
{
	timer0_init();
	uart_init();
	P1 = 0x00;


	while(1)
	{		
		if(_data != 0)
		{	
			switch(_data)
			{
				
				case 10:
					P1_0 = ~P1_0;
					break;
				case 11:
					P1_1 = ~P1_1;
					break;
				case 12:
					P1_2 = ~P1_2;
					break;
				case 13:
					P1_3 = ~P1_3;
					break;
				case 14:
					P1_4 = ~P1_4;
					break;
				case 15:
					P1_5 = ~P1_5;
					break;
				case 16:
					P1_6 = ~P1_6;
					break;
				case 17:
					P1_7 = ~P1_7;
					break;
				case 1:
					P1 = 0x00;
					break;
						
			}
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
void uart_ISR(void) interrupt 4
{
	if(RI == 1)
	{
		_data = SBUF;
		RI = 0;
	}

}