#include "cli.h"
/* 
 * function : prepare_cli
 * parameter : char rx_data that is data is received from RX uart
 * return : function return 1 if rx_data is enter (ascii == 13), else return 0
 * brief : 	 
*/
char prepare_cli(char rx_data)
{
	char result = 0;
	if((rx_data == 13 || rx_data == 10))
	{			
			/* Command Process*/
			parse_cli(buffer,write_index);
			write_index = 0;
			memset(buffer,0,BUFFER_SIZE);
			result = 1;
	}
	else
	{
		buffer[write_index] = rx_data;
		write_index++ ;
	}
	return result;	
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

	unsigned char* temp_cmd = NULL;

	temp_cmd = strtok(buffer,DELIMITED);
	
	while(temp_cmd != NULL)
	{
		
		if(!strcmp(temp_cmd,"Ring") || !strcmp(temp_cmd,"RING"))
		{
			uart_send_str("ATH\r\n");
			P0 = ~P0;
			break;
		}
		else
		{
			temp_cmd = strtok(NULL,DELIMITED);
		}
	}
}


