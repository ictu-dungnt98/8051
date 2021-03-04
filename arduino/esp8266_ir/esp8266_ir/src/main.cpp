#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <string.h>

#include "database.h"
#include "smartIR.h"
#include "uart.h"

remote_t my_ir;

void setup()
{
    uart_init();
    SmartIR_begin();
    eeprom_init();
    
    memset(&my_ir, 0, sizeof(my_ir));

    eeprom_database_loader();
}
/**
 * Luu lenh sau khi da hoc
 * step1: hoc
 * step2: check lenh vua nhan dc
 * step3: save 
 * step4: dung lai
 */
void loop()
{ /* {"brand":21, "power":1, "temp":18, "mode":1, "fan":1, "swing":1} */
    uart_handler();
    SmartIR_loop_handler();
}
