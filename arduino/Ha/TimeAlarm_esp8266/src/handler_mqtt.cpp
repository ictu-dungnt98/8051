#include "handler_mqtt.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "database.h"
#include "led_button.h"
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

float temperature_value;
float humidity_value;

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
void send_system_state(void)
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":%d, \"state\":[%d, %d, %d], \"res\":1}", QUERY_INFOM,
            digitalRead(LED1_PIN), digitalRead(LED2_PIN), digitalRead(LED3_PIN));
    publish_msg(respond);
}

/* {cmd_type:2, "cmd":1, "hour":10, "minutes":10} */
static void handler_set_alarm(JsonDocument& _doc)
{
    char respond[512];
    uint8_t cmd = _doc["cmd"];

    if (m_device.alarm_is_set >= MAX_CMD_ALARM) {
        memset(respond, 0, sizeof(respond));
        sprintf(respond, "{\"cmd_type\":%d, \"res\":0}\n", SET_ALARM);
        publish_msg(respond);
        return;
    }

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "cmd: %d\n", cmd);
    Serial.println(respond);

    /* Do set time alarm here */
    m_device.m_time_alarm[m_device.alarm_is_set].m_time.tm_hour = _doc["hour"];
    m_device.m_time_alarm[m_device.alarm_is_set].m_time.tm_min = _doc["minutes"];
    m_device.m_time_alarm[m_device.alarm_is_set].m_cmd = cmd;
    m_device.alarm_is_set++;

    Serial.print("alarm_is_set!");
    Serial.print("cmd: ");
    Serial.println(m_device.m_time_alarm[m_device.alarm_is_set].m_cmd);

    sync_database_request = 1;

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":%d, \"id\":%d, \"res\":1}\n",
                    SET_ALARM, m_device.alarm_is_set);
    publish_msg(respond);
}

/* {\"cmd_type\":3, \"time_alarm\":[{\"id\":%d, \"cmd\":%d, \"hours\":%d, \"min\":[%d]},{},{}], \"res\":1} */
void handler_get_time_alarm_was_set(void)
{
    char respond[1024];
    char alarms_str[1024];
    char format_alarm[20];
    uint8_t index;

    memset(alarms_str, 0, sizeof(alarms_str));

    for (index = 0; index < MAX_CMD_ALARM; index++) {
        if (m_device.m_time_alarm[index].m_cmd != 0) {
            /* Get Alarm */
            memset(format_alarm, 0, sizeof(format_alarm));
            sprintf(format_alarm, "{\"id\":%d, \"cmd\":%d, \"hours\":%d, \"min\":%d}",
                                index,
                                m_device.m_time_alarm[index].m_cmd,
                                m_device.m_time_alarm[index].m_time.tm_hour,
                                m_device.m_time_alarm[index].m_time.tm_min);

            /* Create Format to make Json */
            strcat(alarms_str, format_alarm);
            strcat(alarms_str, ", ");
        }
    }
    alarms_str[strlen(alarms_str)-2] = '\0';

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":%d, \"time_alarm\":[%s], \"res\":1}\n",
                        GET_TIME_ALARM, alarms_str);

    publish_msg(respond);
    Serial.println(respond);
}

/* {cmd_type:4, "id":1} */
void handler_remove_alarm(JsonDocument& _doc)
{
    uint8_t id = _doc["id"];

    char respond[256];

    /* disable alarm */
    if (id < MAX_CMD_ALARM) {
        m_device.m_time_alarm[id].m_time.tm_hour = 0;
        m_device.m_time_alarm[id].m_time.tm_min = 0;
        m_device.m_time_alarm[id].m_cmd = 0; /* do nothing */

        sync_database_request = 1;

        memset(respond, 0, sizeof(respond));
        sprintf(respond, "{\"cmd_type\":%d, \"res\":1}\n", RESET_ALARM);
        publish_msg(respond);
    }
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

        case SET_ALARM: {
            handler_set_alarm(root);
        } break;

        case GET_TIME_ALARM: {
            handler_get_time_alarm_was_set();
        } break;

        case RESET_ALARM: {
            handler_remove_alarm(root);
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