#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "IRsend.h"

// An IR detector/demodulator is connected to GPIO pin 5 (D1 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = D1;
const uint8_t kIrLed = D2; /*Chân mắt phát hồng ngoại của esp8266  (pin 5) */
unsigned long key_value = 0;
// Control LEDs with the
const int greenPin = 0;   // connected to GPIO pin 0 (D3 on a NodeMCU board).
const int yellowPin = 2;  // connected to GPIO pin 2 (D4 on a NodeMCU board).

IRrecv irrecv(kRecvPin);
IRsend irsend(kIrLed);

decode_results results;

void setup()
{
    Serial.begin(115200);

    while (!Serial)  // Wait for the serial connection to be establised.
        delay(50);

    Serial.println();
    Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
    Serial.println(kRecvPin);

    pinMode(greenPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);

    irsend.begin();
    irrecv.enableIRIn();  // Start the receiver
}
#define _BUG_(...)                                      \
   do                                                   \
   {                                                    \
      Serial.printf("%u\t  %s\t:", __LINE__, __FILE__); \
      Serial.println(__VA_ARGS__);                      \
   } while (0);
#define _BUGF_(...)                                     \
   do                                                   \
   {                                                    \
      Serial.printf("%u\t  %s\t:", __LINE__, __FILE__); \
      Serial.printf(__VA_ARGS__);                       \
   } while (0);
void loop()
{
    uint16_t *p_Raw = NULL;
    if (irrecv.decode(&results)) {

        serialPrintUint64(results.value, HEX);

        p_Raw = resultToRawArray(&results);
        irsend.sendRaw(p_Raw, strlen((char*)p_Raw), 38);

        _BUGF_(resultToSourceCode(&results).c_str());

        irrecv.resume();  // Receive the next value
    }
}