
#include "led.h"

void led_off() {
  digitalWrite(LED1_PIN, HIGH);
}

void led_on() {
  digitalWrite(LED1_PIN, LOW);
}

void led_init(void)
{
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
}