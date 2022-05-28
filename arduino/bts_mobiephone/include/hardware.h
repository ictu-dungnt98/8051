#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <Arduino.h>

#define BTN_PRESSED 0
#define BTN_RELEASED 1

#define SDA_PIN D4
#define SCL_PIN D5

#define BTN_PIN	A0

#define IR_RE 	2
#define IR_TR1 	16
#define IR_TR2 	14

#define RELAY1	15
#define RELAY2	3

#define HC595_D0_PIN		12
#define HC595_RCLK_PIN		14
#define HC595_SCLK_PIN		16

// #define HC595_D0_PIN		0
// #define HC595_RCLK_PIN		13
// #define HC595_SCLK_PIN		12

#define HC595_DATA_PIN	HC595_D0_PIN /* 74HC595 DS pin */
#define HC595_CLK_PIN	HC595_SCLK_PIN /* clock pin */
#define HC595_LATCH_PIN	HC595_RCLK_PIN /* latch pin */


#define Q1_0 (1 << 0)
#define Q1_1 (1 << 1)
#define Q1_2 (1 << 2)
#define Q1_3 (1 << 3)
#define Q1_4 (1 << 4)
#define Q1_5 (1 << 5)
#define Q1_6 (1 << 6)
#define Q1_7 (1 << 7)

#define Q2_0 (1 << 8)
#define Q2_1 (1 << 9)
#define Q2_2 (1 << 10)
#define Q2_3 (1 << 11)
#define Q2_4 (1 << 12)
#define Q2_5 (1 << 13)
#define Q2_6 (1 << 14)
#define Q2_7 (1 << 15)

#define LED_A	(Q1_0)
#define LED_B	(Q1_7)
#define LED_C	(Q1_5)
#define LED_D	(Q1_3)
#define LED_E  	(Q1_2)
#define LED_F	(Q1_1)
#define LED_G  	(Q1_6)
#define LED_DP	(Q1_4)

#define LED_MASK (LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G | LED_DP)

#define LED_2 		Q2_7
#define LED_3 		Q2_6

#define POW_BTN		Q2_0
#define POW_NTC1	Q2_5
#define POW_NTC2	Q2_4

void relay_init(void);
void relay_set(uint8_t relay, uint8_t state);

void m_button_init(void);
void button_scan_task(void);

void init_74hc595(void);
void hc595_write(uint16_t data);
void led_7_seg_write(uint8_t number);

void rtc_init(void);
void rtc_hander(void);

#define os_trace Serial.printf 

#endif