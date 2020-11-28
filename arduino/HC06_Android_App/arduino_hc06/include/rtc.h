#ifndef _RTC_H_
#define _RTC_H_

#include <Arduino.h>
#include "RTClib.h"
#include <time.h>

#define MAX_CMD_ALARM       2

extern RTC_DS1307 rtc;
extern char daysOfTheWeek[7][12];

void rtc_init(void);
void rtc_hander(void);
void uno_caculate_time_device_active_loop(void);

#endif
