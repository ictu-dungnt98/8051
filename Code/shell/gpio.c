#include "gpio.h"

void led_on(unsigned char led_pin)
{
	switch(led_pin)
	{
		case '0':
			P1_0 = 1;
			break;
		case '1':
			P1_1 = 1;
			break;
		case '2':
			P1_2 = 1;
			break;
		case '3':
			P1_3 = 1;
			break;
		case '4':
			P1_4 = 1;
			break;
		case '5':
			P1_5 = 1;
			break;
		case '6':
			P1_6 = 1;
			break;
		case '7':
			P1_7 = 1;
			break;		
	}
}
void led_off(unsigned char led_pin)
{
	switch(led_pin)
	{
		case '0':
			P1_0 = 0;
			break;
		case '1':
			P1_1 = 0;
			break;
		case '2':
			P1_2 = 0;
			break;
		case '3':
			P1_3 = 0;
			break;
		case '4':
			P1_4 = 0;
			break;
		case '5':
			P1_5 = 0;
			break;
		case '6':
			P1_6 = 0;
			break;
		case '7':
			P1_7 = 0;
			break;		
	}
}
void led_callback(unsigned char argc,unsigned char** argv)
{
  unsigned char led_id = argv[0][0];
  if(!strcmp("on",argv[1]))
  {
	uart_send_str("led on\n\r");
  	led_on(led_id);
  }else{
	uart_send_str("led off\n\r");
  	led_off(led_id);
  }
}
