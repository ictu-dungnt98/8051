#ifndef _RTC_H_
#define _RTC_H_

#include <Arduino.h>
#include <time.h>

#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

#include <NTPtimeESP.h>

#define MAX_CMD_ALARM      6

extern strDateTime dateTime;

void rtc_init(void);
void rtc_hander(void);
void rtc_loop(void);

#endif
