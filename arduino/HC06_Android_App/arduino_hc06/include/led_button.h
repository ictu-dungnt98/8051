#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>

#define NUMBER_BUTTON   4

#define BUTTON1_PIN     2
#define BUTTON2_PIN     3
#define BUTTON3_PIN     11
#define BUTTON4_PIN     12

#define BUTTON1         0
#define BUTTON2         1
#define BUTTON3         2
#define BUTTON4         3

#define NUMBER_LED      3

#define LED1_PIN        6
#define LED2_PIN        7
#define LED3_PIN        11

#define TIME_SLICE_TO_READ_BUTTON   10

#define OS_BTN_IS_PRESS_TIME_MAX    200
#define OS_BTN_IS_PRESS_TIME_MIN    50

void led_button_init(void);
void button_scanner(void);

#endif