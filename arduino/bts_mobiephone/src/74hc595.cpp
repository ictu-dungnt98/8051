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
    shiftOut(data, HC595_CLK_PIN, LSBFIRST, data);
    digitalWrite(HC595_LATCH_PIN, HIGH);
}