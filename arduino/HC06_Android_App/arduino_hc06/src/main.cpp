#include <Arduino.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>
#include "RTClib.h"

#include "led_button.h"
#include "lcd1602.h"
#include "hc06.h"
#include "rtc.h"
#include "pzem004t.h"
#include "uno_database.h"

#define TIME_HANDLER_BUTTON         20
#define TIME_HANDLER_LCD            10
#define TIME_HANDLER_HC06           50
#define TIME_HANDLER_RTC            1000
#define TIME_COUNT_TIME_ACTIVE      10000 /* 10s */

extern device_info_t m_device;

static uint32_t time_slice = 0;
static uint32_t time_handler_button_before = 0;
static uint32_t time_handler_lcd_before = 0;
static uint32_t time_handler_hc06_before = 0;
static uint32_t time_handler_rtc_before = 0;
static uint32_t time_count_time_active_before = 0;

void setup()
{
    hc06_init();
    lcd_init();
    rtc_init();
    pzem_init();
    led_button_init();
    Serial.println("system init success");
}

void button_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_button_before > TIME_HANDLER_BUTTON) {
        button_handler();
        time_handler_button_before = time_slice;
    }
}

void lcd_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_lcd_before > TIME_HANDLER_LCD) {
        lcd_handler();
        time_handler_lcd_before = time_slice;
    }
}

void hc06_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_hc06_before > TIME_HANDLER_HC06) {
        hc06_hander();
        time_handler_hc06_before = time_slice;
    }
}

void rtc_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_rtc_before > TIME_HANDLER_RTC) {
        rtc_hander();
        time_handler_rtc_before = time_slice;
    }
}

void uno_count_time_device_active()
{
    time_slice = millis();

    if (time_slice - time_count_time_active_before > TIME_COUNT_TIME_ACTIVE) {
        uno_caculate_time_device_active_loop();
        time_count_time_active_before = time_slice;
    }
}

void loop()
{
    button_loop();
    lcd_loop();
    hc06_loop();
    rtc_loop();
    uno_count_time_device_active();
}

