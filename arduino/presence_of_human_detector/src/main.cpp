#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "led_button.h"
#include "handler_mqtt.h"
#include "rtc.h"
#include "database.h"
#include "sensor.h"

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

// const char* ssid = "Hunonic";
// const char* password = "66668888";

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    Serial.println();
    Serial.print("Dang ket noi wifi ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }

    Serial.print("INFO: IP address: ");
    Serial.println(WiFi.localIP());

    eeprom_init();

    rtc_init();
    mqtt_init();
    led_button_init();
    eeprom_database_loader();

    sensor_init();
}

int is_startup = 1;
void loop()
{
    mqtt_handler();
    button_loop();
    alarm_loop();
    sync_database();
    sensor_read();
}
