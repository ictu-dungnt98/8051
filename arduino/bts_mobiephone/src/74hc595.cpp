#include "hardware.h"

void init_74hc595(void)
{
	pinMode(HC595_LATCH_PIN, OUTPUT);
	pinMode(HC595_DATA_PIN, OUTPUT);
	pinMode(HC595_CLK_PIN, OUTPUT);
}

void hc595_write(uint8_t data)
{
	digitalWrite(HC595_LATCH_PIN, LOW);
	shiftOut(HC595_DATA_PIN, HC595_CLK_PIN, MSBFIRST, data);
    digitalWrite(HC595_LATCH_PIN, HIGH);
	
	os_trace("%02X - %u\n", data, data);
}