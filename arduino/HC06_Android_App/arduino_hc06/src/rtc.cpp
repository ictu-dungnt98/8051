#include "rtc.h"


RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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

void rtc_hander(void)
{

}


