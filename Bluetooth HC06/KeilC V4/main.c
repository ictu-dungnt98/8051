#include <REGX51.H>

volatile char _data = 0;

/****************** UART ******************************/
void uart_init()
{
	TMOD &= 0x0F;
	TMOD |= 0x20;	 // timer1 mode 2 auto update
	SCON = 0x50;	// uart 8bit baudrate is selected by timer1
	TH1 = 0xFD; //9600	
	IE |= 0x90;    // enable interrupt uart 
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

void main()
{
	P3 = 0x00;
	uart_init();
	
	while (1)
	{	
	}				
}

/* UART receive data interrupt */
void uart_ISR(void) interrupt 4
{
	if (RI == 1)
	{
		_data = SBUF;
		RI = 0;
		
		if (_data == '1' || _data == 1)
			P3_7 = 1;
		else if (_data == '0' || _data == 0)
			P3_7 = 0;

		uart_send_byte(_data);
	}
}