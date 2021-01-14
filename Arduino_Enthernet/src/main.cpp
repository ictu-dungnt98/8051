#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include "DHT.h"

#define RELAY1_PIN 12
#define RELAY2_PIN 13

#define BUTTON1_PIN 8
#define BUTTON2_PIN 9

#define LM35_PIN A5
#define DHT11_PIN 2
#define DHTTYPE DHT11

#define W5100_CS 10
#define SDCARD_CS 4

char auth[] = "55yMmZjN1j3PMEWfvq_5Y-bJZkR5qHCS";

float lm35_temperature = 0;
float lm35_temperature_threshold = 0;

float dht11_humidity = 0;
float dht11_humidity_threshold = 0;
float dht11_temperature = 0;
float dht11_temperature_threshold = 0;

static uint8_t relay1_state = 0;
static uint8_t relay2_state = 0;
static uint8_t btn1State = HIGH;
static uint8_t btn2State = HIGH;

#define lm35_virtual_pin V1
#define lm35_virtual_threshold_pin V2
#define dht11_huminity_virtual_pin V3
#define dht11_huminity_threshold_virtual_pin V4
#define dht11_temperature_virtual_pin V5
#define dht11_temperature_threshold_virtual_pin V6

#define relay1_virtual_pin V7
#define relay2_virtual_pin V8

DHT dht(DHT11_PIN, DHTTYPE);
BlynkTimer timer;

void lm35_read();
void dht11_read();
void check_threshold();
void checkPhysicalButton();

void setup()
{
    Serial.begin(9600);

    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);

    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    dht.begin();

    Blynk.begin(auth);
    timer.setInterval(1000L, dht11_read);
    timer.setInterval(1000L, lm35_read);
    timer.setInterval(1000L, check_threshold);
    timer.setInterval(100L, checkPhysicalButton);
}

void loop()
{
    Blynk.run();
    timer.run();
}

void checkPhysicalButton()
{
    if (digitalRead(BUTTON1_PIN) == LOW)
    {
        // btnState is used to avoid sequential toggles
        if (btn1State != LOW)
        {
            Serial.println("BUTTON1_PIN handler");

            // Toggle LED state
            relay1_state = !relay1_state;
            digitalWrite(RELAY1_PIN, relay1_state);

            // Update Button Widget
            Blynk.virtualWrite(relay1_virtual_pin, relay1_state);
        }
        btn1State = LOW;
    }
    else
    {
        btn1State = HIGH;
    }

    if (digitalRead(BUTTON2_PIN) == LOW)
    {
        // btnState is used to avoid sequential toggles
        if (btn2State != LOW)
        {
            Serial.println("BUTTON2_PIN handler");

            // Toggle LED state
            relay2_state = !relay2_state;
            digitalWrite(RELAY1_PIN, relay2_state);

            // Update Button Widget
            Blynk.virtualWrite(relay1_virtual_pin, relay2_state);
        }
        btn2State = LOW;
    }
    else
    {
        btn2State = HIGH;
    }
}

void dht11_read()
{
    dht11_humidity = dht.readHumidity();
    dht11_temperature = dht.readTemperature();

    // Serial.print("Nhiet do: ");
    // Serial.println(dht11_temperature);
    // Serial.print("Do am: ");
    // Serial.println(dht11_humidity);

    Blynk.virtualWrite(dht11_huminity_virtual_pin, dht11_humidity);
    Blynk.virtualWrite(dht11_temperature_virtual_pin, dht11_temperature);
}

void lm35_read()
{
    int lm35_reader = analogRead(LM35_PIN);
    float voltage = lm35_reader * 5.0 / 1024.0;
    lm35_temperature = voltage * 100.0;

    Blynk.virtualWrite(lm35_virtual_pin, lm35_temperature);
    // Serial.print("LM35 nhiet do: ");
    // Serial.println(lm35_temperature);
}

void control_relay(uint8_t relay, uint8_t value)
{
    switch (relay)
    {
    case RELAY1_PIN:
    {
        Blynk.virtualWrite(relay1_virtual_pin, value);
        digitalWrite(RELAY1_PIN, value);
        break;
    }

    case RELAY2_PIN:
    {
        Blynk.virtualWrite(relay2_virtual_pin, value);
        digitalWrite(RELAY2_PIN, value);
        break;
    }

    default:
        break;
    }
}

void check_threshold()
{

    if (lm35_temperature_threshold != 0 &&
        lm35_temperature >= lm35_temperature_threshold)
    {
        control_relay(1, LOW);
    }
    else if (lm35_temperature_threshold != 0 &&
             lm35_temperature < lm35_temperature_threshold)
    {
        control_relay(1, HIGH);
    }

    if (dht11_humidity_threshold != 0 &&
        dht11_humidity_threshold > dht11_humidity)
    {
        control_relay(2, LOW);
    }
    else if (dht11_humidity_threshold != 0 &&
             dht11_humidity_threshold < lm35_temperature_threshold)
    {
        control_relay(2, HIGH);
    }
}

BLYNK_WRITE(lm35_threshold_virtual_pin)
{
    lm35_temperature_threshold = param.asInt();
}

BLYNK_WRITE(dht11_huminity_threshold_virtual_pin)
{
    dht11_humidity_threshold = param.asInt();
}

BLYNK_WRITE(dht11_temperature_threshold_virtual_pin)
{
    dht11_temperature_threshold = param.asInt();
}

BLYNK_WRITE(relay1_virtual_pin)
{
    relay1_state = param.asInt();

    Serial.print("relay1_virtual_pin: ");
    Serial.println(relay1_state);

    if (relay1_state == 0)
    {
        Serial.println("relay1_virtual_pin: LOW");
        digitalWrite(RELAY1_PIN, LOW);
    }
    else
    {
        Serial.println("relay1_virtual_pin: HIGH");
        digitalWrite(RELAY1_PIN, HIGH);
    }
}

BLYNK_WRITE(relay2_virtual_pin)
{
    relay2_state = param.asInt();

    Serial.print("relay2_virtual_pin: ");
    Serial.println(relay2_state);

    if (relay1_state == 0)
    {
        Serial.println("relay2_virtual_pin: LOW");
        digitalWrite(RELAY2_PIN, LOW);
    }
    else
    {
        Serial.println("relay2_virtual_pin: HIGH");
        digitalWrite(RELAY2_PIN, HIGH);
    }
}

BLYNK_CONNECTED()
{
    Serial.println("BLYNK_CONNECTED");
}

BLYNK_DISCONNECTED()
{
    Serial.println("BLYNK_DISCONNECTED");
}