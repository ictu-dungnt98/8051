#include <Arduino.h>
#include "hardware.h"
#include "rtc.h"
#include <time.h>

RTC_DS1307 rtc;
DateTime now;
struct tm m_time_local;
uint8_t alarm_is_set = 0;

void rtc_init(void)
{
        if (!rtc.begin()) {
            while (1);
        }

        if (!rtc.isrunning()) {
            Serial.println();
        }

        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
void rtc_hander(void)
{
    now = rtc.now();

    Serial.println(now.hour());
    Serial.println(now.minute());
    Serial.println(now.second());

}
