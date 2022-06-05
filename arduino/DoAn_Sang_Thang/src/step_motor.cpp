#include "step_motor.h"

#include <Arduino.h>

/* Hardware driver: A4988 */
void step_motor_init()
{
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
}

void motor_loop(void)
{
    for (int x = 0; x < 200; x++)  // Cho chay 1 vong
    {
        digitalWrite(5, HIGH);  // Output high
        delay(10);              // chờ
        digitalWrite(5, LOW);   // Output low
        delay(100);             // chờ
    }
}