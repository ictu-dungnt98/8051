#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "led_button.h"

#define TIME_HANDLER_BUTTON     200

static uint32_t time_slice = 0;
static uint32_t time_scan_button = 0;

LiquidCrystal_I2C lcd(0x3F,16,2); 

void setup()
{
    lcd.init();                    
    lcd.backlight();
    lcd.setCursor(2,0);
    lcd.print("Arduinokit.vn");
    lcd.setCursor(0,1);
    lcd.print("Xin chao cac ban");

    Serial.begin(9600);
    led_button_init();
}

void loop()
{
    time_slice = millis();

    if (time_slice - time_scan_button >TIME_HANDLER_BUTTON) {
        button_scanner();
        time_scan_button = time_slice;
    }
        
}