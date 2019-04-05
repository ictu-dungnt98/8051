#include <REGX51.H>


#define OUTPUT_PIN		P3_7

#define LED_FIX			P0_0
#define LED_START 		P0_1
#define LED_STOP		P0_2

#define LED_SEC			P0_3
#define LED_MIN 		P0_4
#define LED_HOUR		P0_5

#define LED_INCREASE	P0_6
#define LED_DECREASE 	P0_7

#define INCREASE		0
#define DECREASE		1

#define FIX				0
#define START			1
#define STOP			2

#define SEC				1
#define MIN				2
#define HOUR			3

char key[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
volatile unsigned long tick =0, count1 = 0, count2 = 0, check = 0, tac = 0;

unsigned char choose = 0;
unsigned char mode = 0;

unsigned char start = 0;

unsigned char hour = 0;
unsigned char min = 0;
unsigned char sec = 0;

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

void timer1_init()	   // 1ms
{
	TMOD &= ~0xF0;
	TMOD |= 0x10; // 16 bit
	TH1 = 0xFC;
	TL1 = 0x18;
	ET1 = 1;   // enable timer1 interrupt
	EA = 1;
	TR1 = 1;
}
/****************** delay function ******************************/
void delay(unsigned long time)
{
	unsigned long current_tick = tick;
	while(!(tick - current_tick == time));
}

#define LED_7SEG  P2

#define c_hour	P1_0
#define d_hour	P1_1
#define c_min	P1_2
#define d_min	P1_3
#define c_sec	P1_4
#define d_sec	P1_5

void quet_led()
{
/*** SEC ***/
	d_sec = 1;
	LED_7SEG =  key[sec%10];
	delay(5);
	d_sec = 0;

	c_sec = 1;
	LED_7SEG =  key[sec/10];
	delay(5);
	c_sec = 0;
/*** MIN ***/
	d_min = 1;
	LED_7SEG =  key[min%10];
	delay(5);
	d_min = 0;

	c_min = 1;
	LED_7SEG =  key[min/10];
	delay(5);
	c_min = 0;
/*** HOUR ***/
	d_hour = 1;
	LED_7SEG =  key[hour%10];
	delay(5);
	d_hour = 0;

	c_hour = 1;
	LED_7SEG =  key[hour/10];
	delay(5);
	c_hour = 0;			
}

void main()
{
	min = 0;
	hour = 0;
	sec = 30;
	timer0_init();
	timer1_init();
	init_external_IRQ();
	P2 = P0 = 0x00;
	OUTPUT_PIN = 0;
	start = FIX;

	while(1)
	{
		if(P1_7 ==0)
		{
			delay(10);
			if(P1_7 == 0)
			{
				count1 ++;
				if(count1 >3)
				{
					count1 = 0;
				}
				while(P1_7 == 0);
			}
		}
		if(count1 == 3)
		{
			start = STOP;
			LED_STOP = 1;
			LED_START = 0;
			LED_FIX = 0;
			P2 = 0x00;
			OUTPUT_PIN = 0;
		}else if(count1 == 2)
		{
			start = START;
			LED_STOP = 0;
			LED_START = 1;
			LED_FIX = 0;
		}else if(count1 == 1)
		{
			start = FIX;
			LED_STOP = 0;
			LED_START = 0;
			LED_FIX = 1;
		}
		if(start == FIX)
		{
			P0_0 = 1;
			if(P1_6 == 0)
			{
				delay(10);
				if(P1_6 ==0)
				{
					count2 ++;
					if(count2 > 2)
					{
						count2 = 0;
					}
					while(P1_6 == 0);
				}
			}
			if(count2 == 2)			// HOUR
			{
				choose = HOUR;
				LED_HOUR = 1;
				LED_MIN = 0;
				LED_SEC = 0;

			}else if(count2 == 1)   // MIN
			{
				choose = MIN;
				LED_HOUR = 0;
				LED_MIN = 1;
				LED_SEC = 0;

			}else if(count2 == 0)    // SEC
			{
				choose = SEC;
				LED_HOUR = 0;
				LED_MIN = 0;
				LED_SEC = 1;

			}
		}	
		if(start == START)
		{
								
		}
		quet_led();
		
	}				
}
/* selection mode button */
void External_ISR0() interrupt 0
{
	if(start == FIX)
	{
		if(mode == INCREASE)
		{
			mode = DECREASE;
			LED_DECREASE = 1;
			LED_INCREASE = 0;
		}else
		{
			mode = INCREASE;
			LED_DECREASE = 0;
			LED_INCREASE = 1;
		}
	}
}
/* Increse or Decrease button */
void External_ISR1() interrupt 2
{
	if(start == FIX)
	{
		if(mode == INCREASE) /* increase */
		{
			if(choose == HOUR)
			{
				hour++;
			}else if(choose == MIN)
			{
				min++;
			}else if(choose == SEC)
			{
				sec++;
			}
			P0_0 = ~P0_0;
		}else /* decrease */
		{
			if(choose == HOUR)
			{
				
				if(hour == 0)
				{
					hour = 23;
				}else if(hour >0)
				{
					hour--;
				}
			}else if(choose == MIN)
			{
				
				if(min == 0)
				{
					min = 59;
					if(hour >0)
					{
						hour --;
					}
				}else if(min >0)
				{
					min--;
				}
			}else if(choose == SEC)
			{
				if(sec  == 0)
				{
					sec = 59;
					if(min >0 )
					{	
						min --;
					}
				}else if(sec >0)
				{
					sec--;
				}
			}
			P0_0 = ~P0_0;
		}
		P0_0 = ~P0_0;
	}
	
	
}
void timer0_ISR(void) interrupt 1
{
	 tick++;
	 TH0 = 0xFC;
	 TL0 = 0x18;
}
void timer1_ISR(void) interrupt 3
{
	if(start == START)
	{
		tac++;
		if(tac >999)
		{
			tac = 0;	
			if(sec == 0)
			{			
				if(min == 0)
				{	
					if(hour == 0)
					{			
						OUTPUT_PIN = 1;
					}else if(hour >0)
					{
						min = 59;
						hour --;
					}
				}else if(min  > 0)
				{
					sec = 59;
					min--;
				}
			}else if(sec >0)
			{
				sec--;
			}
		}
	}
	TH1 = 0xFC;
	TL1 = 0x18;
}