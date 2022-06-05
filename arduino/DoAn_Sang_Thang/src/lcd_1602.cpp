
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

static uint32_t _so_vong = 0;
static uint8_t _lcd_update = 0;

void lcd_init(void)
{
	lcd.init();  // initialize the lcd
    lcd.backlight();
}

void lcd_loop()
{
	if (_lcd_update) {
		lcd.clear();
		lcd.setCursor(0, 0);

		char data[50];
		memset(data, 0, sizeof(data));
		sprintf(data, "So vong: %d", _so_vong);
		lcd.print(data);

		_lcd_update = 0;
	}
}

void set_so_vong(uint32_t so_vong)
{
	_so_vong = so_vong;
	_lcd_update = 1;
}