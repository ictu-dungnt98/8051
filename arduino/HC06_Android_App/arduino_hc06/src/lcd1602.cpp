#include "lcd1602.h"

LiquidCrystal_I2C lcd(0x27,16,2);

void lcd_init(void)
{
    lcd.init();                    
    lcd.backlight();
    // lcd.setCursor(2,0);
    lcd.print("Arduinokit.vn");
    // lcd.setCursor(0,1);
    lcd.print("Xin chao cac ban");
}

void lcd_handler(void)
{

}