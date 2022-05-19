#include <Arduino.h>
#include "hardware.h"

void relay_init(void)
{
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}

void relay_set(uint8_t relay, uint8_t state)
{
  if (relay == 1)
  {
	digitalWrite(RELAY1, state);
  }
  else if (relay == 2)
  {
	digitalWrite(RELAY2, state);
  }
}