#ifndef _GPIO_H_
#define _GPIO_H_

#include <REGX51.H>
#include <string.h>
#include <stdlib.h>
#include "./uart.h"

void led_callback(unsigned char argc,unsigned char** argv);
void led_on(unsigned char led_pin);
void led_off(unsigned char led_pin);


#endif