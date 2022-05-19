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
    now = rtc.now();

    os_trace("%d\n", now.hour());
    os_trace("%d\n", now.minute());
    os_trace("%d\n", now.second());
}
