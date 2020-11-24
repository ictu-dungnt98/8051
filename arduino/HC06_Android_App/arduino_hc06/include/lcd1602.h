#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void lcd_init(void);
void lcd_handler(void);

#endif