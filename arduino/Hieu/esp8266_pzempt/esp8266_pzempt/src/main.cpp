#include <Arduino.h>

#include "database.h"
#include "handler_mqtt.h"
#include "rtc.h"
#include "uart.h"

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

// const char* ssid = "Hunonic";
// const char* password = "66668888";

void setup()
{
    uart_init();
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }

    Serial.print("INFO: IP address: ");
    Serial.println(WiFi.localIP());

    rtc_init();

    database_init();
    database_load();

    mqtt_init();
}

void loop()
{
    alarm_loop();
    mqtt_handler();

    uart_handler();
    sync_database();
}
