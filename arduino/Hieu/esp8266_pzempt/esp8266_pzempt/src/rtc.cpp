#include "rtc.h"
#include "database.h"
#include "uart.h"

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 19800;
NTPClient timeClient(ntpUDP, "google.com.vn", utcOffsetInSeconds);

NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
strDateTime dateTime;

extern device_info_t m_device;
struct tm m_time_local;
uint8_t alarm_is_set = 0;

bool is_new_month(void);
bool is_new_year(void);

void reset_data_for_new_month(void);

void rtc_init(void)
{
    dateTime = NTPch.getNTPtime(7.0, 0);
}
void alarm_loop()
{
    static uint32_t time_handler_alarm_before = 0;
    
    if (millis() - time_handler_alarm_before < 1000)
        return;
    time_handler_alarm_before = millis();

    dateTime = NTPch.getNTPtime(7.0, 0);
    Serial.printf("%d:%d:%d %d/%d/%d\n",
                dateTime.hour, dateTime.minute, dateTime.second,
                dateTime.day, dateTime.month, dateTime.year);

    if (dateTime.month > 0 && dateTime.month <= 12 &&
        dateTime.day > 0 && dateTime.day < 31)
        m_device.current_month = dateTime.month;

    /* check new month */
    if (is_new_month()) {
        reset_data_for_new_month();
        memset(m_device.data[dateTime.month].energy, 0, 100);
        m_device.data[dateTime.month].water = 0;
    }
    /* check end of year */
    if (is_new_year()) {
        reset_database();
    }
}
bool is_new_month(void)
{
    if (!dateTime.hour&& !dateTime.minute && !dateTime.second)
        if (dateTime.day == 1) /* new month */
            return 1;
    
    return 0;
}

bool is_new_year(void)
{
    if (is_new_month())
        if (dateTime.month == 1) /* new year */
            return 1;
    
    return 0;
}

void reset_data_for_new_month(void)
{
    Serial.printf("{\"cmd\":%d}", CMD_RESET_DATA);
}