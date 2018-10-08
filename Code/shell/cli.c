#include "cli.h"

cli_t* head_cmd = NULL ;
cli_t* current_cmd = NULL;

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
	

	if(command == NULL)	 /* if cannot allocate memmory */
	{
		uart_send_str("Memmory overflow !\n");
		while(1);
	}
/*************************  DEBUG ****************************/
#if 1
	uart_send_str("\n\r1.CLI::cli_init()\n\rhelp_command: ");
	uart_send_str(help_command);
	uart_send_str("\n\r\n\r");
#endif
/************************************************************/


/***  
* after run underneath code , CODE begin error
***/	
	command->cmd = help_command;

/*************************  DEBUG ****************************/
#if 1
	uart_send_str("\n\r1.CLI::cli_init()\n\rhelp_command: ");
	uart_send_str(help_command);
	uart_send_str("\n\r\n\r");
#endif
/************************************************************/	
	
/*************************  DEBUG ****************************/	
#if 1
	uart_send_str("\n\r2.CLI::cli_init()\n\rcommand->cmd: ");
	uart_send_str(command->cmd);
	uart_send_str("\n\r\n\r");
#endif
/************************************************************/
	
	command->num_parameter = 0;
	command->callback = help;
	command->next_cmd = NULL;
	
	head_cmd = command;
	current_cmd = command;
	
}

void cli_add(cli_t* list_cmd, unsigned char num_cmd)
{
	unsigned char i = 0;
	cli_t *new_node = (cli_t*)malloc(sizeof(cli_t));
	
/*************************  DEBUG ****************************/
#if 1
	uart_send_str("\n\r4.CLI::cli_add()\n\rcurrent_cmd: ");
	uart_send_str(current_cmd->cmd);
	uart_send_str("\n\r\n\r");
#endif
/************************************************************/
	for(i=0; i< num_cmd; i++)
	{
		
/*************************  DEBUG ****************************/		
#if 1
	uart_send_byte(i+48);
	uart_send_str("\n\r***.CLI::cli_add()\n\rlist_cmd[i]: ");
	uart_send_str(list_cmd[i].cmd);
	uart_send_str("\n\r\n\r");
#endif
/************************************************************/		
		
		/* Make new node */
		new_node->cmd = list_cmd[i].cmd;
		new_node->num_parameter = list_cmd[i].num_parameter;
		new_node->callback = list_cmd[i].callback;

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
