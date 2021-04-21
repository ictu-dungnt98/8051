#include "handler_mqtt.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"

#include "led_button.h"

const char* mqtt_client_id = "ir_controller";
const char* mqtt_server_ip = "broker.hivemq.com";
const char* mqtt_user = "admin";
const char* mqtt_password = "admin";
int mqtt_port = 1883;

const char* mqtt_topic_sub = "sub";
const char* mqtt_topic_pub = "pub";

WiFiClient espClient;
PubSubClient client(espClient);

 float temperature_value;
 float humidity_value;

void send_system_state(void)
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":105, \"state\":[%d, %d, %d], \"temp\":%f, \"humi\":%f, \"res\":OK}", digitalRead(LED1_PIN),
            digitalRead(LED2_PIN), digitalRead(LED3_PIN), temperature_value, humidity_value);

    client.publish(mqtt_topic_pub, respond);
}

void send_system_startup_state(void)
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":105, \"state\":[%d, %d, %d], \"temp\":%f, \"humi\":%f, \"res\":OK}", digitalRead(LED1_PIN),
            digitalRead(LED2_PIN), digitalRead(LED3_PIN), temperature_value, humidity_value);

    client.publish(mqtt_topic_pub, respond);
}

void report_current_state()
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":%d, \"state\":[%d, %d, %d], \"sensor\":[%f,%f], \"res\":OK}",
                        CONTROL_IO, digitalRead(LED1_PIN), digitalRead(LED2_PIN),
                        digitalRead(LED3_PIN), temperature_value, humidity_value);

    client.publish(mqtt_topic_pub, respond);
}

void parse_data(JsonDocument& root)
{
    uint8_t jsonCmd = root["cmd_type"];
    uint8_t cmd;

    switch (jsonCmd) {
        case QUERY_INFOM: {
            send_system_state();
        } break;

        case CONTROL_IO: {
            cmd = root["cmd"];
            control_pin(cmd);
        } break;

        default:
            break;
    }
}

/* Json */
void mqtt_callback(char* p_toppic, uint8_t* p_data, unsigned int length)
{
    /* parse data */
    Serial.println((char*)p_data);

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, p_data);

    if (error) {
        Serial.print(F("Parse Json Failed.\n"));
        return;
    }

    parse_data(doc);
}

void reconnect()
{
    while (!client.connected()) {
        Serial.println("Dang ket noi MQTT...");
        // Connect MQTT
        if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
            Serial.println("Da ket noi xong MQTT");  // Ket noi xong , hien thi
            client.subscribe(mqtt_topic_sub);
        } else {
            Serial.print("No connect: ");
            Serial.print(client.state());
            Serial.println("Doi 5 giay");
            delay(5000);
        }
    }
}


void mqtt_init(void)
{
    client.setServer(mqtt_server_ip, mqtt_port);
    client.setCallback(mqtt_callback);
}

void mqtt_handler()
{
    if (!client.connected()) {
        reconnect();
    }

    client.loop();
}