#include "sensor.h"
#include "database.h"
#include "DHT.h"
#define DHT11_PIN 4
#define LIGHT_SENSOR A0
#define LIGHT_THRESHOLD 1000 /* adc value threshold */

#define DHTTYPE DHT11  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
 
DHT dht(DHT11_PIN, DHTTYPE);

float temperature, huminity;

void sensor_init()
{
    dht.begin();    
}

void sensor_read()
{
    static uint32_t time_handler_before = 0;

    if (millis() - time_handler_before < 5000) /* 5s */
        return;

    time_handler_before = millis();

    huminity = dht.readHumidity();
    temperature = dht.readTemperature();

    Serial.printf("Temper: %.2f\n", temperature);
    Serial.printf("huminity: %.2f\n", huminity);

    if (m_device.sensor.threshold_type == 0)
        return;

    if (m_device.sensor.threshold_type == 1) /* humi over */
    {
        if (huminity > m_device.sensor.huminity_threshold)
            control_pin(m_device.sensor.action);
    }
    else if (m_device.sensor.threshold_type == 2) /* temper over */
    {
        if (temperature > m_device.sensor.temperature_threshold)
            control_pin(m_device.sensor.action);
    }
    else if (m_device.sensor.threshold_type == 3) /* humi lower */
    {
            if (huminity < m_device.sensor.huminity_threshold)
            control_pin(m_device.sensor.action);
    }
    else if (m_device.sensor.threshold_type == 4) /* temper lower */
    {
        if (temperature < m_device.sensor.temperature_threshold)
            control_pin(m_device.sensor.action);
    }
}