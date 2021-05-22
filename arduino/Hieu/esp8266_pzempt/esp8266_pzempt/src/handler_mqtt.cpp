#include "handler_mqtt.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "database.h"
#include "rtc.h"

#define CMD_USER_READ_DATA  5
#define CMD_USER_READ_CUR_DATA  6
#define CMD_RESET_FACTORY       7

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

void publish_data_to_sever(JsonDocument& root);
void publish_cur_data_to_sever(JsonDocument& root);
void parse_data(JsonDocument& root);

void publish_msg(char* msg)
{
    client.publish(mqtt_topic_pub, msg);
}

/* Json */
void mqtt_callback(char* p_toppic, uint8_t* p_data, unsigned int length)
{
    /* parse data */
    // Serial.printf("%s\n", (char*)p_data);

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
            Serial.print(client.state());
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


void parse_data(JsonDocument& root)
{
    uint8_t cmd_type = root["cmd_type"];
    char buff[1024];
    memset(buff, 0, 1024);

    switch (cmd_type) {
        case CMD_USER_READ_DATA: {
            publish_data_to_sever(root);
        } break;

        case CMD_USER_READ_CUR_DATA: {
            publish_cur_data_to_sever(root);
        } break;

        case CMD_RESET_FACTORY: {
            reset_database();
            sprintf(buff, "%s", "{\"cmd\": 1}");
            Serial.println(buff);

            memset(buff, 0, 1024);
            sprintf(buff, "{\"cmd_type\": %d, \"result\":1}", cmd_type);
            publish_msg(buff);
        } break;

        default:
            break;
    }
}


void publish_data_to_sever(JsonDocument& root)
{
    char buff[1024];
    memset(buff, 0, 1024);

    uint8_t cmd_type = root["cmd_type"];

    sprintf(buff, "{\"cmd_type\": %d, \
    \"energy\": [\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \
\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\"], \
\"water\": [%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d]}",
            cmd_type,
            m_device.data[0].energy,
            m_device.data[1].energy,
            m_device.data[2].energy,
            m_device.data[3].energy,
            m_device.data[4].energy,
            m_device.data[5].energy,
            m_device.data[6].energy,
            m_device.data[7].energy,
            m_device.data[8].energy,
            m_device.data[9].energy,
            m_device.data[10].energy,
            m_device.data[11].energy,

            m_device.data[0].water,
            m_device.data[1].water,
            m_device.data[2].water,
            m_device.data[3].water,
            m_device.data[4].water,
            m_device.data[5].water,
            m_device.data[6].water,
            m_device.data[7].water,
            m_device.data[8].water,
            m_device.data[9].water,
            m_device.data[10].water,
            m_device.data[11].water);

    publish_msg(buff);
}

void publish_cur_data_to_sever(JsonDocument& root)
{
    char buff[1024];
    memset(buff, 0, 1024);

    uint8_t cmd_type = root["cmd_type"];

    sprintf(buff, "{\"cmd_type\": %d, \"energy\": \"%s\", \"water\": %d}",
            cmd_type,
            m_device.data[m_device.current_month-1].energy,
            m_device.data[m_device.current_month-1].water);

    publish_msg(buff);
}