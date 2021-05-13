#include "uart.h"
#include "database.h"
#include "rtc.h"

#include <Arduino.h>
#include <ArduinoJson.h>



char hc06_rx_queue[512];
uint16_t p_hc06_rx_data = 0;

void handler_data(char* command);
void update_database(JsonDocument &doc);

void uart_init(void)
{
    Serial.begin(115200);
    while (!Serial);  // Wait for the serial connection to be establised.}
}

void uart_handler(void)
{
    char ch;

    p_hc06_rx_data = 0;
    memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));

    if (Serial.available() > 0) {
        while (Serial.available() > 0) {
            ch = Serial.read();

            hc06_rx_queue[p_hc06_rx_data] = ch;
            p_hc06_rx_data++;

            delay(1); /*1ms */
        }

        Serial.print("recieved: ");
        Serial.println(hc06_rx_queue);

        /* Handler data recieved */
        handler_data(hc06_rx_queue);

        p_hc06_rx_data = 0;
        memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));
    }
}


void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        Serial.print(F("Decode fail\n"));
        Serial.println(error.c_str());
        return;
    }

    if ((uint8_t)doc["cmd"] == CMD_POST_DATA) {
        update_database(doc);
    }
}
extern uint8_t current_month;

uint32_t current_water_used = 0;
String current_energy;

void update_database(JsonDocument &doc)
{
    current_energy = doc["energy"].as<String>();
    current_water_used = doc["water"].as<int16_t>();

    Serial.printf("dateTime.month : %d\n", m_device.current_month);
    memset(m_device.data[m_device.current_month-1].energy, 0, 100);
    sprintf(m_device.data[m_device.current_month-1].energy, "%s", current_energy.c_str());
    m_device.data[m_device.current_month-1].water = current_water_used;
    sync_database_request = 1;
}
