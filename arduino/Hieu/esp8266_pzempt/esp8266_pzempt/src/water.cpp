#include <Arduino.h>

#define SENSOR D3

static uint32_t previousMillis = 0;
static float calibrationFactor = 4.5;
static uint8_t pulse1Sec = 0;
static float flowRate;

unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;

volatile uint8_t pulseCount;

void IRAM_ATTR pulseCounter() {
    pulseCount++;
}

void water_sensor_setup(void)
{
    pinMode(SENSOR, INPUT_PULLUP);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;

    Serial.println("attachInterrupt");
    attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}

void read_water_loop(void)
{
    if (millis() - previousMillis > 1000) {
        pulse1Sec = pulseCount;
        pulseCount = 0;

        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = millis();

        // Divide the flow rate in litres/minute by 60 to determine how many litres have
        // passed through the sensor in this 1 second interval, then multiply by 1000 to
        // convert to millilitres.
        flowMilliLitres = (flowRate / 60) * 1000;
        flowLitres = (flowRate / 60);

        // Add the millilitres passed in this second to the cumulative total
        totalMilliLitres += flowMilliLitres;
        totalLitres += flowLitres;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(float(flowRate));  // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t");  // Print tab space

        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);
        Serial.print("mL / ");
        Serial.print(totalLitres);
        Serial.println("L");
    }
}