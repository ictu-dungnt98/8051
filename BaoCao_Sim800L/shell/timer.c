#include "timer.h"

volatile unsigned long tick = 0;

/****************** TIMER ******************************/
void timer0_init()	   // 1ms
{
	TMOD &= 0xF0;
	TMOD |= 0x01; // 16 bit

	TH0 = 0xFC;
	TL0 = 0x67;

	IE |= 0x82;   // enable timer0 interrupt
	
	TR0 = 1;
}
/****************** delay function ******************************/
void delay(unsigned long time)
{
	unsigned long current_tick = tick;
	while(!(tick - current_tick == time));
}
