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

/*************************   Function   ***************************************/
void uart_send_byte(char data_);
void uart_send_str(const char* str);
void test_callback(char argc,char** argv);

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
void led_callback(unsigned char argc,unsigned char** argv)
{
  unsigned char led_id = argv[0][0] - '0';
  if(!strcmp("on",argv[1]))
  {
  	led_on(led_id);
  }else{
  	led_off(led_id);
  }
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

/*****************************************************************/
/*************************** CLI ********************************/
/***************************************************************/
typedef struct _cli_t_
{
	const char* cmd;	/* name of command */
	unsigned int num_parameter;
	const void (*callback)(unsigned char argc,unsigned char** argv);
	struct _cli_t_ *next_cmd;
}cli_t;

static cli_t* head_cmd = NULL ;
static cli_t* current_cmd = NULL;

cli_t list_cmd[NUM_CLI] =	{
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
							};

void test_callback(char argc,char** argv)
{
//	uart_send_str("\nTest function callback!\n");
	
	cli_t* temp_cmd = head_cmd;

	char i = 48;
	while(temp_cmd != NULL)
	{
		i++;
		uart_send_byte(i);
		uart_send_str("\n\r");
		uart_send_str(temp_cmd->cmd);
		uart_send_str("\n\r");

		temp_cmd = temp_cmd->next_cmd;
	}			
}

void help(unsigned char argc, unsigned char** argv)
{
	cli_t* temp_cmd = head_cmd;

	while(temp_cmd != NULL)
	{
		if(argc == 0)
		{
			uart_send_str(temp_cmd->cmd);
			uart_send_str("\n\r\t");
			uart_send_str((char*)temp_cmd->num_parameter);
			uart_send_str("\n\r");				
		}
		else
		{
			uart_send_str(temp_cmd->cmd);
			uart_send_str("\n\r\t");
			uart_send_str((char*)temp_cmd->num_parameter);
			uart_send_str("\n\r");

			break;
		}

		temp_cmd = temp_cmd->next_cmd;
	}

}
void cli_init(void)
{
	unsigned int  num_input = 0;
	const char* help_command = "help";
	cli_t* command = (cli_t*)malloc(sizeof(cli_t));
#if 1
	uart_send_str("\n\r1.CLI::cli_init()\n\rhelp_command: ");
	uart_send_str(help_command);
	uart_send_str("\n\r\n\r");
#endif
	if(command == NULL)	 /* if cannot allocate memmory */
	{
		uart_send_str("Memmory overflow !\n");
		while(1);
	}
	command->cmd = help_command;
	command->num_parameter = 0;
	command->callback = help;
	command->next_cmd = NULL;
	
	head_cmd = command;
	current_cmd = command;

#if 1
	uart_send_str("\n\r2.CLI::cli_init()\n\rhead_cmd: ");
	uart_send_str(head_cmd->cmd);
	uart_send_str("\n\r\n\r");
#endif
	
}

void cli_add(cli_t* list_cmd, unsigned char num_cmd)
{
	unsigned char i = 0;
	cli_t *new_node = (cli_t*)malloc(sizeof(cli_t));

#if 1
	uart_send_str("\n\r4.CLI::cli_add()\n\rcurrent_cmd: ");
	uart_send_str(current_cmd->cmd);
	uart_send_str("\n\r\n\r");
#endif

	for(i=0; i< num_cmd; i++)
	{
			new_node->cmd = list_cmd[i].cmd;
			new_node->num_parameter = list_cmd[i].num_parameter;
			new_node->callback = list_cmd[i].callback;
		
#if 1
uart_send_byte(i+48);
uart_send_str("\n\r***.CLI::cli_add()\n\rlist_cmd[i]: ");
uart_send_str(list_cmd[i].cmd);
uart_send_str("\n\r\n\r");
#endif


		current_cmd->next_cmd = new_node;

		current_cmd = current_cmd->next_cmd;
		current_cmd->next_cmd = NULL;
		
#if 1
		uart_send_byte(i+48);
		uart_send_str("\n\r***.CLI::cli_add()\n\rcurrent_cmd: ");
		uart_send_str(current_cmd->next_cmd->cmd);
		uart_send_str("\n\r\n\r");
#endif	
		
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
	cli_t* temp_cmd = head_cmd;
	unsigned char* temp_str = NULL;
	unsigned char** input_parameter;
	unsigned char num_input = 0;

	temp_str = strtok(buffer,DELIMITED);

	#if 1
	uart_send_str("\n\rCLI :: parse_cli()\n\rhead_cmd : ");
	uart_send_str(head_cmd->cmd);
	uart_send_str("\n\r\n\r");
	#endif
	
	while(temp_cmd != NULL)
	{
		if(!strcmp(temp_cmd->cmd, temp_str))   /* if found function */
		{
			input_parameter = (unsigned char**)malloc(sizeof(unsigned int) * temp_cmd->num_parameter);
			num_input = 0;
			temp_str = strtok(NULL,DELIMITED);
			while(temp_str != NULL)
			{
				if(num_input > temp_cmd->num_parameter)
				{
				 	break;
				}
				else
				{
					input_parameter[num_input] = temp_str;
					num_input++;
					temp_str = strtok(NULL,DELIMITED);
				}	
			}
		   	break;
		}
		temp_cmd = temp_cmd->next_cmd;
	}

	if(temp_cmd != NULL)   /* If have this function */
	{
	   if(num_input == temp_cmd->num_parameter)	/* if syntax of function is correct */
	   {
	   	  temp_cmd->callback(num_input,input_parameter);
	   }
	   else
	   {
	   		uart_send_str("! ERROR : syntax error !\n");
	   }
	
	}
	else
	{
		uart_send_str("! ERROR : Function not exist !\n");
	}

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
		parse_cli(buffer,write_index);
		write_index = 0;
		memset(buffer,0,BUFFER_SIZE);

		result = 1;
	}
	else if(rx_data == 8)
	{
		if(write_index >0)
		{
			write_index-- ;
			buffer[write_index] = 0;
		}
	}
	else
	{
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

/********************************************************************************/
/****************************    MAIN   ****************************************/
/******************************************************************************/
void main()
{
	timer0_init();
	uart_init();
	P1 = 0x00;

	cli_init();

	uart_send_str("\n\r3main :: cli_init()\n\rhead_cmd: ");
	uart_send_str(head_cmd->cmd);
	uart_send_str("\n\r\n\r");


	cli_add(list_cmd,NUM_CLI);

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