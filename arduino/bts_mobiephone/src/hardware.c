#include <Arduino.h>
#include "hardware.h"

uint32_t sys_get_tick_ms(void)
{
	return millis();
}
