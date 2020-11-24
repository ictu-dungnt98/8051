#ifndef _RTC_H_
#define _RTC_H_

#include <Arduino.h>
#include "RTClib.h"

extern RTC_DS1307 rtc;
extern char daysOfTheWeek[7][12];

void rtc_init(void);
void rtc_hander(void);

#endif
