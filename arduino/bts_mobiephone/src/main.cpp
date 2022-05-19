#include <Arduino.h>
#include "hardware.h"
#include "rtc.h"
#include "database.h"
#include "handler_mqtt.h"

void setup()
{
	Serial.begin(115200);
	while (!Serial);

	rtc_init();
	init_74hc595();

	os_trace("dungnt98 BTS\n");
}

void loop() 
{
  	button_scan_task();
}