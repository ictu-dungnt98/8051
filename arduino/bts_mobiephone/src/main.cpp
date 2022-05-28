#include <Arduino.h>

#include "database.h"
#include "handler_mqtt.h"
#include "hardware.h"
#include "rtc.h"
#include "smartIR.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    os_trace("dungnt98 BTS\n");

    // rtc_init();
    init_74hc595();
	// m_button_init();
	// smartir_setup_io();

	hc595_write(0x000F);
}

void loop()
{
	// button_scan_task();
	// rtc_hander();
	// smartir_loop_handler();
}