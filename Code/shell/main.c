#include <REGX51.H>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE		50
#define DELIMITED		"_ "
#define NUM_CLI			2
volatile unsigned long tick = 0;
volatile char _data = 0;

static char buffer[BUFFER_SIZE] = "";
static unsigned char write_index = 0;
static unsigned char read_index = 0;


void uart_send_str(char* str);

/*********************************  GPIO  **************************************/
void led_on(unsigned char led_pin)
{
	switch(led_pin)
	{
		case 0:
			P1_0 = 1;
			break;
		case 1:
			P1_1 = 1;
			break;
		case 2:
			P1_2 = 1;
			break;
		case 3:
			P1_3 = 1;
			break;
		case 4:
			P1_4 = 1;
			break;
		case 5:
			P1_5 = 1;
			break;
		case 6:
			P1_6 = 1;
			break;
		case 7:
			P1_7 = 1;
			break;		
	}
}
void led_off(unsigned char led_pin)
{
	switch(led_pin)
	{
		case 0:
			P1_0 = 0;
			break;
		case 1:
			P1_1 = 0;
			break;
		case 2:
			P1_2 = 1;
			break;
		case 3:
			P1_3 = 0;
			break;
		case 4:
			P1_4 = 0;
			break;
		case 5:
			P1_5 = 0;
			break;
		case 6:
			P1_6 = 0;
			break;
		case 7:
			P1_7 = 0;
			break;		
	}
}
void led_callback(unsigned char* argc,unsigned char** argv)
{
//  unsigned char led_id = argv[0][0] - '0';
//  if(!strcmp("on",(const char*)argv[1]))
//  {
//  	led_on(led_id);
//  }else{
//  	led_off(led_id);
//  }
}
void test_callback(char* argc,char** argv)
{
	uart_send_str("\nTest function callback!\n");
}
/*****************************************************************/
/*************************** CLI ********************************/
/***************************************************************/
typedef struct _cli_t_
{
	const char* cmd;	/* name of command */
	unsigned int num_parameter;
	const void (*callback)(unsigned char* argc,unsigned char** argv);
	struct _cli_t_ *next_cmd;
}cli_t;

static cli_t* head_cmd = NULL ;

cli_t list_cmd[NUM_CLI] =
{
	{
		"led", /* name of cmd */
		2, /* num_input_par */
		led_callback,
		NULL
	},
	{
		"test",
		0,
		test_callback,
		NULL
	}
}
/* 
 * function : parse_cli
 * parameter : 	function take 2 parameter, 
 *				unsigned char* buffer, that is string command of user trasmit through Terminal 
 *				unsigned char len_buffer, lengh of command
 * brief : *function is not written finish*
*/
void parse_cli(unsigned char* buffer, unsigned char len_buffer)
{
	cli_t* temp_cmd;
	unsigned char* _cmd = NULL;
	unsigned char num_input = 0;
}
/* 
 * function : prepare_cli
 * parameter : char rx_data that is data is received from RX uart
 * return : function return 1 if rx_data is enter (ascii == 13), else return 0
 * brief : 	 
*/
char prepare_cli(char rx_data)
{
	char result = 0;
	if(rx_data == 13)
	{
		/* Command Process*/
		//parse_cli(buffer,write_index);
		write_index = 0;
		memset(buffer,0,BUFFER_SIZE);
		result = 1;
	}
	else if(rx_data == 8)
	{
		write_index-- ;
		buffer[write_index] = 0;
	}
	else{
		buffer[write_index] = rx_data;
		write_index++ ;
	}

	return result;	
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
/****************** delay function ******************************/
void delay(unsigned long time)
{
	unsigned long current_tick = tick;
	while(!(tick - current_tick == time));
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
			if(prepare_cli(_data))
			{
				uart_send_str("\n\r");
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
		uart_send_byte(_data);
		RI = 0;
	}
}