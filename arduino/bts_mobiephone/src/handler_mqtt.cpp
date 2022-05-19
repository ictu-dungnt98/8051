#include "handler_mqtt.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "database.h"
#include "hardware.h"
#include "rtc.h"

const char* mqtt_client_id = "ir_controller";
const char* mqtt_server_ip = "broker.hivemq.com";
const char* mqtt_user = "admin";
const char* mqtt_password = "admin";
int mqtt_port = 1883;

const char* mqtt_topic_sub = "sub";
const char* mqtt_topic_pub = "pub";

WiFiClient espClient;
PubSubClient client(espClient);

void parse_data(JsonDocument& root)
{
    uint8_t cmd_type = root["cmd_type"];

    switch (cmd_type) {
        default:
            break;
    }
}

/* Json */
void mqtt_callback(char* p_toppic, uint8_t* p_data, unsigned int length)
{
    /* parse data */
    os_trace("%s\n", (char*)p_data);

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, p_data);

    if (error) {
        os_trace("Parse Json Failed.\n");
        return;
    }

    parse_data(doc);
}

void mqtt_init(void)
{
    client.setServer(mqtt_server_ip, mqtt_port);
    client.setCallback(mqtt_callback);
}

void mqtt_handler()
{
    if (!client.connected()) {
        while (!client.connected()) {
            if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
                os_trace("Connected mqtt\n");
                client.subscribe(mqtt_topic_sub);
            }
        }
    }

    client.loop();
}