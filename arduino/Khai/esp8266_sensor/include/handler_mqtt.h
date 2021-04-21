#ifndef _MQTT_H_
#define _MQTT_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"


void mqtt_init(void);
void mqtt_handler(void);

void report_current_state();
void send_system_startup_state(void);
void send_system_state(void);

#endif /* _MQTT_H_ */