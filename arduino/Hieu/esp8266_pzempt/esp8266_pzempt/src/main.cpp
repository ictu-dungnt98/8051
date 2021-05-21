#include <Arduino.h>

#include "database.h"
#include "handler_mqtt.h"
#include "rtc.h"
#include "uart.h"
/* BLYNK */
#include <BlynkSimpleEsp8266.h>

#define water_sensor_blynk_pin            V5
#define power_sensor_blynk_pin                 V4
char auth[] = "AK_mtAJMVhL3ceBBnt63QWJZ1DY-7qBi";

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

// const char* ssid = "Hunonic";
// const char* password = "66668888";

void setup()
{
    uart_init();
    
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(100);
    // }

    // Serial.print("INFO: IP address: ");
    // Serial.println(WiFi.localIP());

    rtc_init();

    database_init();
    database_load();

    mqtt_init();
    Blynk.begin(auth, ssid, password);
}

void loop()
{
    Blynk.run();
    // mqtt_handler();

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
        memset(m_device.data[m_device.current_month-1].energy, 0, 100);
        sprintf(m_device.data[m_device.current_month-1].energy, "%s", current_energy.c_str());
    }

    m_device.data[m_device.current_month-1].water = current_water_used;

    Serial.printf("water: %d\n", m_device.data[m_device.current_month-1].water);
    Serial.printf("engergy: %f\n", atof(m_device.data[m_device.current_month-1].energy));

    Blynk.virtualWrite(water_sensor_blynk_pin,
                        m_device.data[m_device.current_month-1].water);
    Blynk.virtualWrite(power_sensor_blynk_pin,
                        atof(m_device.data[m_device.current_month-1].energy));
    
    sync_database_request = 1;
}
