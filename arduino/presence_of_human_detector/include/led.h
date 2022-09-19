#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>

#include "m_typedef.h"

#define ON HIGH
#define OFF LOW

#define NUMBER_LED 1
#define LED1_PIN 2

void led_init(void);
void control_pin(uint8_t cmd);
void gpio_toggle(uint8_t pin);

#endif