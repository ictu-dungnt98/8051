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
}