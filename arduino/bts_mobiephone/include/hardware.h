#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <Arduino.h>

#define SDA_PIN D4
#define SCL_PIN D5

#define BTN_PIN	A0

#define IR_RE 	2
#define IR_TR1 	16
#define IR_TR1 	14

#define RELAY1	15
#define RELAY2	3

#define HC595_DATA_PIN	D0 /* 74HC595 DS pin */
#define HC595_CLK_PIN	12 /* clock pin */
#define HC595_LATCH_PIN	13 /* latch pin */

#define os_trace Serial.printf 

uint32_t sys_get_tick_ms(void);

void relay_init(void);
void relay_set(uint8_t relay, uint8_t state);

#endif