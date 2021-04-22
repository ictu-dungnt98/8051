#include "rtc.h"
#include <time.h>

#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

#include "led_button.h"
#include "m_typedef.h"
#include "database.h"

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 19800;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

extern device_info_t m_device;

struct tm m_time_local;
uint8_t alarm_is_set = 0;

void rtc_init(void)
{
        timeClient.begin();
}

static void time_alarm()
{
    uint8_t i;

    for (i = 0; i < MAX_CMD_ALARM; i++) {
        if (timeClient.getHours() == m_device.m_time_alarm[i].m_time.tm_hour &&
            timeClient.getMinutes() == m_device.m_time_alarm[i].m_time.tm_min)
        {
            Serial.print("Do Alarm Hanlder with cmd: ");
            Serial.println(m_device.m_time_alarm[i].m_cmd);
            control_pin(m_device.m_time_alarm[i].m_cmd);
        }
        else
        {
            Serial.println(m_device.m_time_alarm[i].m_time.tm_hour);
            Serial.println(m_device.m_time_alarm[i].m_time.tm_min);
        }
    }
}

#define TIME_HANDLER_RTC            1000
static uint32_t time_handler_alarm_before = 0;
void alarm_loop()
{
    if (millis() - time_handler_alarm_before > TIME_HANDLER_RTC) {
        timeClient.update();

        if (m_device.alarm_is_set) {
            time_alarm();
        }

        time_handler_alarm_before = millis();
    }
}