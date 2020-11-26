#include "rtc.h"
#include <time.h>
#include "m_typedef.h"
#include "led_button.h"

RTC_DS1307 rtc;
DateTime now;
struct tm m_time_local;
m_alarm_t m_time_alarm[MAX_CMD_ALARM];
uint8_t alarm_is_set = 0;

void rtc_init(void)
{
    if (Serial) {
        if (! rtc.begin()) {
            Serial.print("Couldn't find RTC");
            while (1);
        }

        if (!rtc.isrunning()) {
            Serial.print("RTC is NOT running!");
            Serial.println();
        }

        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}


static void time_alarm() {
    uint8_t i;

    // for (i = 0; i < MAX_CMD_ALARM; i++) {
    //     if (memcmp(&m_time_local, &m_time_alarm[i].m_time, sizeof(struct tm)) == 0)
    //     {
    //         control_device(m_time_alarm[i].m_cmd);
    //     }
    // }

    for (i = 0; i < MAX_CMD_ALARM; i++) {
        if (m_time_local.tm_hour == m_time_alarm[i].m_time.tm_hour
            && m_time_local.tm_min == m_time_alarm[i].m_time.tm_min
            && m_time_local.tm_sec == m_time_alarm[i].m_time.tm_sec)
        {
            control_device(m_time_alarm[i].m_cmd);
        }
    }
}

void rtc_hander(void)
{
    now = rtc.now();

    m_time_local.tm_hour = now.hour();
    m_time_local.tm_min = now.minute();
    m_time_local.tm_sec = now.second();
    // m_time_local.tm_mday = now.day();
    // m_time_local.tm_mon = now.month();
    // m_time_local.tm_year = now.year();

    if (alarm_is_set) {
        time_alarm();
    }
}


