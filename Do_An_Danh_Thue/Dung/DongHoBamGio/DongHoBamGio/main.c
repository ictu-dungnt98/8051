#include <REGX51.H>

char key[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
volatile unsigned long count = 0;
volatile unsigned char tick = 0;

unsigned char start = 0;

unsigned char min = 0;
unsigned char sec = 0;
unsigned char tac = 0;

void init_external_IRQ()
{
	   IT0 = 1;
	   IT1 = 1;
	   EX0 = 1;
	   EX1 = 1;
	   EA = 1;
}

/****************** TIMER ******************************/
void timer0_init()	   // 1ms
{
	TMOD &= ~0x0F;
	TMOD |= 0x01; // 16 bit
	TH0 = 0xFC;
	TL0 = 0x18;
	ET0 = 1;   // enable timer0 interrupt
	EA = 1;
	TR0 = 1;
}
/****************** delay function ******************************/
void delay(unsigned long time)
{
	unsigned long current_count = count;
	while(!(count - current_count == time));
}

#define LED		P2

#define c_min	P1_0
#define d_min	P1_1
#define c_sec	P1_2
#define d_sec	P1_3
#define c_tac	P1_4
#define d_tac	P1_5


void quet_led()
{
	d_tac = 1;
	LED =  key[tac%10];
	delay(5);
	d_tac = 0;
	/****/
	c_tac = 1;
	LED =  key[tac/10];
	delay(5);
	c_tac = 0;

	d_sec = 1;
	LED =  key[sec%10];
	delay(5);
	d_sec = 0;
	/****/
	c_sec = 1;
	LED =  key[sec/10];
	delay(5);
	c_sec = 0;

	d_min = 1;
	LED =  key[min%10];
	delay(5);
	d_min = 0;
	/****/
	c_min = 1;
	LED =  key[min/10];
	delay(5);
	c_min = 0;			
}

void main()
{
	timer0_init();
	init_external_IRQ();
	P2 = P1 = 0x00;
	start = 0;

	while(1)
	{	
		quet_led();		
	}				
}
void External_ISR0() interrupt 0
{
	start = ~ start;
}
void External_ISR1() interrupt 2
{
	tick = 0;
	tac = 0;
	sec = 0;
	min = 0;
}
void timer0_ISR(void) interrupt 1
{
	 
	 count++;
	 if(start)
	 {
		 tick++;
		 if(tick >9)
		 {
		 	tick = 0;
		 	tac += 1;
			if(tac > 99)
			{
				tac = 0;
				sec += 1;
				if(sec >59)
				{
					sec = 0;
					min ++;
				}
			}
		 }
	 }
	 TH0 = 0xFC;
	 TL0 = 0x18;
}