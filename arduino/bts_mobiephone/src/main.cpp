#include <Arduino.h>

#include "database.h"
#include "handler_mqtt.h"
#include "hardware.h"
#include "rtc.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    os_trace("dungnt98 BTS\n");

    rtc_init();
    init_74hc595();
	m_button_init();

	hc595_write(0x01);
	hc595_write(0x01);
}

void loop()
{
	button_scan_task();
	delay(200);
}