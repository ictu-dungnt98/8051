#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "led_button.h"
#include "handler_mqtt.h"
#include "database.h"

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }

    eeprom_init();
    eeprom_database_loader();

    mqtt_init();
    led_button_init();
}

void loop()
{
    mqtt_handler();
    button_loop();
    sync_database();
}
