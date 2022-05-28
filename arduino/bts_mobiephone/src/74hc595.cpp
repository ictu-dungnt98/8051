#include "hardware.h"

static uint16_t hc595_data = 0x00;

static uint16_t led_arr[]  = {
	(0x00FF & ~(LED_A|LED_B|LED_C|LED_D|LED_E|LED_F)),
	(0x00FF & ~(LED_B|LED_C)), 						
	(0x00FF & ~(LED_A|LED_B|LED_G|LED_D|LED_E)),
	(0x00FF & ~(LED_A|LED_B|LED_G|LED_C|LED_D)),
	(0x00FF & ~(LED_F|LED_G|LED_B|LED_C)),
	(0x00FF & ~(LED_A|LED_F|LED_G|LED_C|LED_D)),
	(0x00FF & ~(LED_A|LED_F|LED_G|LED_C|LED_D|LED_E)),
	(0x00FF & ~(LED_A|LED_B|LED_C)),
	(0x00FF & ~(LED_A|LED_B|LED_C|LED_D|LED_E|LED_F|LED_G)),
	(0x00FF & ~(LED_A|LED_B|LED_C|LED_D|LED_F|LED_G)),
};

void init_74hc595(void)
{
	pinMode(HC595_LATCH_PIN, OUTPUT);
	pinMode(HC595_DATA_PIN, OUTPUT);
	pinMode(HC595_CLK_PIN, OUTPUT);

	hc595_write(0x00);
}

void hc595_write(uint16_t data)
{
	digitalWrite(HC595_LATCH_PIN, LOW);

	/* write data to ic */
	shiftOut(HC595_DATA_PIN, HC595_CLK_PIN, MSBFIRST, (data >> 8) & 0xFF);
	shiftOut(HC595_DATA_PIN, HC595_CLK_PIN, MSBFIRST, data & 0xFF);

    digitalWrite(HC595_LATCH_PIN, HIGH);
}

void led_7_seg_write(uint8_t number)
{
	if (number > 9)
		number = 9;

	/* clear led mask */
	hc595_data &= ~LED_MASK;
	/* write data to register */
	hc595_data |= led_arr[number];

	hc595_write(hc595_data);
}