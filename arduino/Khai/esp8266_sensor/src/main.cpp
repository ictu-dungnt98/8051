#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "DHT.h"

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

const char* mqtt_client_id = "ir_controller";
const char* mqtt_server_ip = "broker.hivemq.com";
const char* mqtt_user = "admin";
const char* mqtt_password = "admin";
int mqtt_port = 1883;

const char* mqtt_topic_sub = "sub";
const char* mqtt_topic_pub = "pub";

const int led1 = D3;
const int led2 = D4;
const int led3 = D5;
const int button1 = D0;
const int button2 = D1;
const int button3 = D2;

WiFiClient espClient;
PubSubClient client(espClient);

#define ON HIGH
#define OFF LOW

#define CMD_CONTROL_PIN 0
#define CMD_READ_INFORM 105

#define DHT11_PIN       D7          
#define LIGHT_SENSOR    A0

#define LIGHT_THRESHOLD     1000 /* adc value threshold */

DHT dht(DHT11_PIN, DHT11);

float temperature_value = 0;
float humidity_value = 0;
uint16_t light_adc = 0;

typedef enum {
    CH1_ON = 1,
    CH1_OFF = 2,
    CH2_ON = 3,
    CH2_OFF = 4,
    CH3_ON = 5,
    CH3_OFF = 6,
} hc06_ctrl_t;

void send_system_state(void)
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":105, \"state\":[%d, %d, %d], \"temp\":%f, \"humi\":%f, \"res\":OK}", digitalRead(led1),
            digitalRead(led2), digitalRead(led3), temperature_value, humidity_value);

    client.publish(mqtt_topic_pub, respond);
}

void send_system_startup_state(void)
{
    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":105, \"state\":[%d, %d, %d], \"temp\":%f, \"humi\":%f, \"res\":OK}", digitalRead(led1),
            digitalRead(led2), digitalRead(led3), temperature_value, humidity_value);

    client.publish(mqtt_topic_pub, respond);
}

/* {"cmd_type":0, "cmd":1} */
void control_pin(uint8_t cmd)
{
    switch (cmd) {
        case CH1_ON: {
            digitalWrite(led1, ON);
        } break;

        case CH1_OFF: {
            digitalWrite(led1, OFF);
        } break;

        case CH2_ON: {
            digitalWrite(led2, ON);
        } break;

        case CH2_OFF: {
            digitalWrite(led2, OFF);
        } break;

        case CH3_ON: {
            digitalWrite(led3, ON);
        } break;

        case CH3_OFF: {
            digitalWrite(led3, OFF);
        } break;

        default:
            break;
    }

    /* respond to server */

    char respond[100];
    memset(respond, 0, sizeof(respond));

    sprintf(respond, "{\"cmd_type\":0, \"state\":[%d, %d, %d], \"sensor\":[%f,%f], \"res\":OK}", digitalRead(led1),
            digitalRead(led2), digitalRead(led3), temperature_value, humidity_value);

    client.publish(mqtt_topic_pub, respond);
}

void parse_data(JsonDocument& root)
{
    uint8_t jsonCmd = root["cmd_type"];
    uint8_t cmd;

    switch (jsonCmd) {
        case CMD_READ_INFORM: {
            send_system_state();
        } break;

        case CMD_CONTROL_PIN: {
            cmd = root["cmd"];
            control_pin(cmd);
        } break;

        default:
            break;
    }
}

/* Json */
void callback(char* p_toppic, uint8_t* p_data, unsigned int length)
{
    /* parse data */
    Serial.println((char*)p_data);

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, p_data);

    if (error) {
        Serial.print(F("Decode fail\n"));
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

void read_sensor(void)
{
    static uint32_t m_time  = 0;

    if (millis() - m_time < 100)
        return;

    m_time = millis();

    humidity_value = dht.readHumidity();
    temperature_value = dht.readTemperature();
    light_adc = analogRead(LIGHT_SENSOR);

    if (light_adc > LIGHT_THRESHOLD) {
        /* Turn on Led */
        control_pin(CH1_ON);
    } else {
        /* Turn off Led */
        control_pin(CH1_OFF);
    }
}

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

    client.setServer(mqtt_server_ip, mqtt_port);
    client.setCallback(callback);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(button1, INPUT);
    pinMode(button1, INPUT);
    pinMode(button1, INPUT);
}

int is_startup = 1;
void loop()
{
    if (!client.connected()) {
        reconnect();
    }

    client.loop();

    if (is_startup) {

    }

    /* read sensor */
    read_sensor();
}
