#include <Arduino.h>

#include "database.h"
#include "handler_mqtt.h"
#include "rtc.h"
#include "uart.h"

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

void setup()
{
    uart_init();
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    Serial.print("INFO: IP address: ");
    Serial.println(WiFi.localIP());

    rtc_init();

    database_init();
    database_load();

    mqtt_init();

    sprintf(m_device.data[0].energy, "121");
    sprintf(m_device.data[1].energy, "115");
    sprintf(m_device.data[2].energy, "129");
    sprintf(m_device.data[3].energy, "134");
    sprintf(m_device.data[3].energy, "126");

    m_device.data[0].water = 1010;
    m_device.data[1].water = 1240;
    m_device.data[2].water = 1168;
    m_device.data[3].water = 1175;
    m_device.data[4].water = 1237;
}

void loop()
{
    rtc_loop();
    mqtt_handler();

    uart_handler();
    sync_database();
}

extern uint8_t current_month;

uint32_t current_water_used = 0;
String current_energy;
void update_database(JsonDocument &doc)
{
    current_energy = doc["energy"].as<String>();
    current_water_used = doc["water_mililites"].as<int16_t>();

    if (atof(current_energy.c_str()) != 0) {
        memset(m_device.data[5].energy, 0, 100);
        sprintf(m_device.data[5].energy, "%s", current_energy.c_str());
    }

    m_device.data[5].water = current_water_used;
    sync_database_request = 1;
}
