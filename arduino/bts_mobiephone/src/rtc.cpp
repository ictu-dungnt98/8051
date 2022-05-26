#include "rtc.h"

#include <Arduino.h>
#include <time.h>

#include "hardware.h"

RTC_DS1307 rtc;
DateTime now;
struct tm m_time_local;
uint8_t alarm_is_set = 0;

void rtc_init(void)
{
    if (!rtc.begin()) {
        os_trace("Couldn't find RTC\n");
    }

    if (!rtc.isrunning()) {
		os_trace("RTC is NOT running!\n");
    }

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void rtc_hander(void)
{
	static uint32_t old_tick = 0;
	if (millis() - old_tick < 1000)
		return;
	old_tick = millis();

    now = rtc.now();
	uint8_t hour = now.hour();
	uint8_t minute = now.minute();
	uint8_t second = now.second();

    os_trace("%d:%d:%d\n", hour, minute, second);
}
