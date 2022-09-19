#include "handler_mqtt.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "led.h"

const char* mqtt_client_id = "ir_controller";
const char* mqtt_server_ip = "broker.hivemq.com";
const char* mqtt_user = "admin";
const char* mqtt_password = "admin";
int mqtt_port = 1883;

const char* mqtt_topic_sub = "sub";
const char* mqtt_topic_pub = "pub";

WiFiClient espClient;
PubSubClient client(espClient);

void publish_msg(char* msg)
{
    os_trace(msg);
    client.publish(mqtt_topic_pub, msg);
}

/* {"cmd_type":0, "cmd":1} */
static void control_device(JsonDocument& root)
{
    uint8_t cmd = root["cmd"];
    control_pin(cmd);
}

// {cmd_type:1}
extern float temperature, huminity;
void send_system_state(void)
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond,
            "{\"cmd_type\":%d, \"state\":[%d, %d, %d], \"res\":1}",
            QUERY_INFOM, digitalRead(LED1_PIN), digitalRead(LED2_PIN),
            digitalRead(LED3_PIN));
    publish_msg(respond);
}

void parse_data(JsonDocument& root)
{
    uint8_t cmd_type = root["cmd_type"];

    switch (cmd_type) {
        case CONTROL_IO: {
            control_device(root);

        } break;

        case QUERY_INFOM: {
            send_system_state();
        } break;

        default:
            break;
    }
}

/* Json */
void mqtt_callback(char* p_toppic, uint8_t* p_data, unsigned int length)
{
    /* parse data */
    Serial.printf("%s\n", (char*)p_data);

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