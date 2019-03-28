#ifndef _TIMER_H_
#define _TIMER_H_

#include <REGX51.H>

extern volatile unsigned long tick;

void timer0_init(void);
void delay(unsigned long time);


#endif