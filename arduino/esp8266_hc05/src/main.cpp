#include <Arduino.h>
#include "hc06.h"
#include "motor.h"

#define TIME_READ_HC06           50
static uint32_t time_read_hc06_before = 0;
static uint32_t time_process_data_before = 0;

void setup() {
	motor_init();
  	hc06_init();
}

static void hc06_loop()
{
    if (millis() - time_read_hc06_before > TIME_READ_HC06) {
        time_read_hc06_before = millis();

        hc06_read_data();
    }

	if (millis() - time_process_data_before > TIME_READ_HC06) {
		time_process_data_before = millis();

		hc06_process_data();
	}
}

void loop() {
  	hc06_loop();
}