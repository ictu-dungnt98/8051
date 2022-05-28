#include "smartIR.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <IRremoteESP8266.h>
#include <string.h>
#include "macro.h"
#include "database.h"
#include "hardware.h"

#define F_IR 38 /* MHz */

const uint16_t kRecvPin = IR_RE; /*Chân mắt thu hồng ngoại của esp8266  (pin 5) */
const uint8_t kIrLed = IR_TR1;    /*Chân mắt phát hồng ngoại của esp8266  (pin 5) */

IRrecv irrecv(kRecvPin);
IRsend irsend(kIrLed);

decode_results results;
uint8_t learn_ir = true;
uint16_t *p_Raw = NULL;
uint16_t RawLength = 0;

void smartir_setup_io(void)
{
    pinMode(IR_RE, OUTPUT);
    pinMode(IR_TR1, OUTPUT);
}

void learnir()
{
    if (irrecv.decode(&results)) {
        // uint16_t tempLen = getCorrectedRawLength(&results);

        if (results.decode_type != UNKNOWN) {
            Serial.print(F("learnir\n"));
            _BUGF_(resultToSourceCode(&results).c_str());

            p_Raw = resultToRawArray(&results);
            RawLength = getCorrectedRawLength(&results);
        }

        irrecv.resume();
    }
}

void smartir_loop_handler(void)
{
    if (true == learn_ir) {
        learnir();
    }
}