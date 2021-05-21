#ifndef _UART_H_
#define _UART_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#define CMD_POST_DATA   0
#define CMD_RESET_DATA  1
#define CMD_READ_DATA   2

void uart_init(void);
void uart_handler(void);




#endif /* _UART_H_ */