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

#define HC595_RCLK_PIN		13
#define HC595_SCLK_PIN		12

#define HC595_DATA_PIN	0 /* 74HC595 DS pin */
#define HC595_CLK_PIN	HC595_SCLK_PIN /* clock pin */
#define HC595_LATCH_PIN	HC595_RCLK_PIN /* latch pin */

void relay_init(void);
void relay_set(uint8_t relay, uint8_t state);

void m_button_init(void);
void button_scan_task(void);

void init_74hc595(void);
void hc595_write(uint8_t data);

void rtc_init(void);
void rtc_hander(void);

#define os_trace Serial.printf 

#endif