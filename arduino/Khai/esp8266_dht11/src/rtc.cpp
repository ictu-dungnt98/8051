#include "rtc.h"
#include <time.h>

#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

#include <NTPtimeESP.h>

#include "led_button.h"
#include "m_typedef.h"
#include "database.h"

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 19800;
NTPClient timeClient(ntpUDP, "google.com.vn", utcOffsetInSeconds);

NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
strDateTime dateTime;

extern device_info_t m_device;
struct tm m_time_local;
uint8_t alarm_is_set = 0;

void rtc_init(void)
{
    // timeClient.begin();
    // timeClient.setUpdateInterval();

    dateTime = NTPch.getNTPtime(7.0, 0);
}

static void time_alarm()
{
    uint8_t i;

    for (i = 0; i < MAX_CMD_ALARM; i++) {
        if (dateTime.hour == m_device.m_time_alarm[i].m_time.tm_hour &&
            dateTime.minute == m_device.m_time_alarm[i].m_time.tm_min)
        {
            if (m_device.m_time_alarm[i].m_cmd >0 && 
                m_device.m_time_alarm[i].m_cmd <= 6)
            {
                Serial.printf("Alarm: %d:%d - cmd: %d", 
                            dateTime.hour,
                            dateTime.minute,
                            m_device.m_time_alarm[i].m_cmd);

                control_pin(m_device.m_time_alarm[i].m_cmd);
            }
        }
    }
}

#define TIME_HANDLER_RTC            1000
static uint32_t time_handler_alarm_before = 0;
void alarm_loop()
{
    if (millis() - time_handler_alarm_before > TIME_HANDLER_RTC) {
        // timeClient.update();
        // Serial.printf("%d:%d:%d\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
        dateTime = NTPch.getNTPtime(7.0, 0);
        Serial.printf("%d:%d:%d\n", dateTime.hour, dateTime.minute, dateTime.second);
        if (m_device.alarm_is_set) {
            time_alarm();
        }

        time_handler_alarm_before = millis();
    }
}