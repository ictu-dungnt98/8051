#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>

#define ON HIGH
#define OFF LOW

#define LED1_PIN 2

void led_off();
void led_on();
void led_init(void);

#endif