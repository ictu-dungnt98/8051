#include <Arduino.h>
#include <BlynkSimpleEthernet.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "DHT.h"

#define RELAY1_PIN 11
#define RELAY2_PIN 12

#define BUTTON1_PIN 6
#define BUTTON2_PIN 7

#define DHT11_PIN 2
#define DHTTYPE DHT11

#define W5100_CS 10

// LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F,16,2);

char auth[] = "5SsX9k6e7WrNcSEXx6gxIpHexKH5tRSJ";

float dht11_humidity = 0;
float dht11_humidity_threshold = 0;
float dht11_temperature = 0;
float dht11_temperature_threshold = 0;

static uint8_t relay1_state = 0;
static uint8_t relay2_state = 0;
static uint8_t btn1State = HIGH;
static uint8_t btn2State = HIGH;

#define dht11_huminity_virtual_pin V5
#define dht11_huminity_threshold_virtual_pin V4
#define dht11_temperature_virtual_pin V3
#define dht11_temperature_threshold_virtual_pin V6

#define relay1_virtual_pin V7
#define relay2_virtual_pin V8

DHT dht(DHT11_PIN, DHTTYPE);
BlynkTimer timer;

void dht11_read();
void checkPhysicalButton();

void setup()
{
    Serial.begin(9600);

    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);

    lcd.init();  // initialize the lcd
    lcd.backlight();

    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    dht.begin();

    Blynk.begin(auth);
    timer.setInterval(1000L, dht11_read);
    timer.setInterval(100L, checkPhysicalButton);
}

void loop()
{
    Blynk.run();
    // timer.run();
    dht11_read();
    checkPhysicalButton();
    lcd.clear();
}
void checkPhysicalButton()
{
    if (digitalRead(BUTTON1_PIN) == LOW) {
        // btnState is used to avoid sequential toggles
        if (btn1State != LOW) {
            Serial.println("BUTTON1_PIN handler");

            // Toggle LED state
            relay1_state = !relay1_state;
            digitalWrite(RELAY1_PIN, relay1_state);

            // Update Button Widget
            Blynk.virtualWrite(relay1_virtual_pin, relay1_state);
        }
        btn1State = LOW;
    } else {
        btn1State = HIGH;
    }

    if (digitalRead(BUTTON2_PIN) == LOW) {
        // btnState is used to avoid sequential toggles
        if (btn2State != LOW) {
            Serial.println("BUTTON2_PIN handler");

            // Toggle LED state
            relay2_state = !relay2_state;
            digitalWrite(RELAY2_PIN, relay2_state);

            // Update Button Widget
            Blynk.virtualWrite(relay2_virtual_pin, relay2_state);
        }
        btn2State = LOW;
    } else {
        btn2State = HIGH;
    }
}

void dht11_read()
{
    dht11_humidity = dht.readHumidity();
    dht11_temperature = dht.readTemperature();

    Blynk.virtualWrite(dht11_huminity_virtual_pin, dht11_humidity);
    Blynk.virtualWrite(dht11_temperature_virtual_pin, dht11_temperature);
}

void control_relay(uint8_t relay, uint8_t value)
{
    switch (relay) {
        case RELAY1_PIN: {
            Blynk.virtualWrite(relay1_virtual_pin, value);
            digitalWrite(RELAY1_PIN, value);
            break;
        }

        case RELAY2_PIN: {
            Blynk.virtualWrite(relay2_virtual_pin, value);
            digitalWrite(RELAY2_PIN, value);
            break;
        }

        default:
            break;
    }
}

BLYNK_WRITE(dht11_huminity_threshold_virtual_pin) { dht11_humidity_threshold = param.asInt(); }

BLYNK_WRITE(dht11_temperature_threshold_virtual_pin) { dht11_temperature_threshold = param.asInt(); }

BLYNK_WRITE(relay1_virtual_pin)
{
    relay1_state = param.asInt();

    Serial.print("relay1_virtual_pin: ");
    Serial.println(relay1_state);

    if (relay1_state == 0) {
        Serial.println("relay1_virtual_pin: LOW");
        digitalWrite(RELAY1_PIN, LOW);
    } else {
        Serial.println("relay1_virtual_pin: HIGH");
        digitalWrite(RELAY1_PIN, HIGH);
    }
}

BLYNK_WRITE(relay2_virtual_pin)
{
    relay2_state = param.asInt();

    Serial.print("relay2_virtual_pin: ");
    Serial.println(relay2_state);

    if (relay2_state == 0) {
        Serial.println("relay2_virtual_pin: LOW");
        digitalWrite(RELAY2_PIN, LOW);
    } else {
        Serial.println("relay2_virtual_pin: HIGH");
        digitalWrite(RELAY2_PIN, HIGH);
    }
}

BLYNK_CONNECTED() { Serial.println("BLYNK_CONNECTED"); }

BLYNK_DISCONNECTED() { Serial.println("BLYNK_DISCONNECTED"); }