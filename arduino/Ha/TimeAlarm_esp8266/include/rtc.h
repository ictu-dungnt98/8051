#ifndef _RTC_H_
#define _RTC_H_

#include <Arduino.h>
#include <time.h>

#define MAX_CMD_ALARM      3

void rtc_init(void);
void rtc_hander(void);
void alarm_loop(void);

#endif
