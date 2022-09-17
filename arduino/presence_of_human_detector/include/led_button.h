#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>
#include "m_typedef.h"

#define ON HIGH
#define OFF LOW

#define NUMBER_CHANNEL   2
#define NUMBER_BUTTON   NUMBER_CHANNEL

#define BUTTON1_PIN      16
#define BUTTON2_PIN      5
#define BUTTON3_PIN      0

#define BUTTON1         0
#define BUTTON2         1
#define BUTTON3         2

#define NUMBER_LED      NUMBER_CHANNEL
#define LED1_PIN        12
#define LED2_PIN        13
#define LED3_PIN        14

#define TIME_SLICE_TO_READ_BUTTON   10

#define OS_BTN_IS_PRESS_TIME_MAX    5000
#define OS_BTN_IS_PRESS_TIME_MIN    20

void led_button_init(void);
void button_loop(void);
void control_pin(uint8_t cmd);

void gpio_toggle(uint8_t pin);

#endif