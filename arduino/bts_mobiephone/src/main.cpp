#include <Arduino.h>
#include "hardware.h"

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
	while (!Serial);
	

	os_trace("dungnt98 BTS\n");
}

void loop() {
  // put your main code here, to run repeatedly:
}