#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <string.h>

#include "database.h"
#include "smartIR.h"
#include "uart.h"

state_machine_t state_machine;
remote_t m_remote;

void setup()
{
    uart_init();
    eeprom_init();
    SmartIR_begin();

    eeprom_database_loader();
}

uint16_t *p_Raw = NULL;
uint8_t yes_no;
uint8_t cmd_control;

void loop()
{ /* {"brand":21, "power":1, "temp":18, "mode":1, "fan":1, "swing":1} */
    uart_handler();
    SmartIR_loop_handler();
}