#include "rtc.h"
#include <time.h>
#include "m_typedef.h"
#include "led_button.h"
#include "uno_database.h"

extern device_info_t m_device;


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

/* run each 10 second */
void uno_caculate_time_device_active_loop(void)
{
    if (digitalRead(LED1_PIN)) {
        m_device.time_active_on_day[0].tm_sec += 10; /* 10s */

        if (m_device.time_active_on_day[0].tm_sec >= 60) {
            m_device.time_active_on_day[0].tm_min ++;
            m_device.time_active_on_day[0].tm_sec = 0;

            if (m_device.time_active_on_day[0].tm_min >= 60) {
                m_device.time_active_on_day[0].tm_hour ++;
                m_device.time_active_on_day[0].tm_min = 0;
            }
        }
    }

    if (digitalRead(LED2_PIN)) {
        m_device.time_active_on_day[1].tm_sec += 10; /* 10s */

        if (m_device.time_active_on_day[1].tm_sec >= 60) {
            m_device.time_active_on_day[1].tm_min ++;
            m_device.time_active_on_day[1].tm_sec = 0;

            if (m_device.time_active_on_day[1].tm_min >= 60) {
                m_device.time_active_on_day[1].tm_hour ++;
                m_device.time_active_on_day[1].tm_min = 0;
            }
        }
    }

    if (digitalRead(LED3_PIN)) {
        m_device.time_active_on_day[2].tm_sec += 10; /* 10s */

        if (m_device.time_active_on_day[2].tm_sec >= 60) {
            m_device.time_active_on_day[2].tm_min ++;
            m_device.time_active_on_day[2].tm_sec = 0;

            if (m_device.time_active_on_day[2].tm_min >= 60) {
                m_device.time_active_on_day[2].tm_hour ++;
                m_device.time_active_on_day[2].tm_min = 0;
            }
        }
    }

    /* handler store and reset data day after day */
}

void rtc_hander(void)
{
    now = rtc.now();

    m_time_local.tm_hour = now.hour();
    m_time_local.tm_min = now.minute();
    m_time_local.tm_sec = now.second();

    m_time_local.tm_wday = now.dayOfTheWeek();
    m_time_local.tm_mday = now.day();
    m_time_local.tm_mon = now.month();
    m_time_local.tm_year = now.year();

    if (alarm_is_set) {
        time_alarm();
    }

    /* Check End of day - 0h, update time active every day */
    if ((now.hour() == 0) && (now.minute() == 0) && (now.second() == 0))
    {
        if (now.dayOfTheWeek() == 0) { /* End of the week */
            m_device.time_active_in_week[0] += m_device.time_active_on_day[0].tm_hour;
            m_device.time_active_in_week[1] += m_device.time_active_on_day[1].tm_hour;
            m_device.time_active_in_week[2] += m_device.time_active_on_day[2].tm_hour;

            /* Count time active on month */
            m_device.time_active_in_month[0] += m_device.time_active_in_week[0];
            m_device.time_active_in_month[1] += m_device.time_active_in_week[1];
            m_device.time_active_in_month[2] += m_device.time_active_in_week[2];

            /* Reset data of week to use for new week */
            m_device.time_active_in_week[0] = 0;
            m_device.time_active_in_week[1] = 0;
            m_device.time_active_in_week[2] = 0;

            /* Reset data to use for new month */
            if (now.day() == 1) {
                m_device.time_active_in_month[0] = 0;
                m_device.time_active_in_month[1] = 0;
                m_device.time_active_in_month[2] = 0;
            }
            
        } else {
            m_device.time_active_in_week[0] += m_device.time_active_on_day[0].tm_hour;
            m_device.time_active_in_week[1] += m_device.time_active_on_day[1].tm_hour;
            m_device.time_active_in_week[2] += m_device.time_active_on_day[2].tm_hour;
        }
    }
}


