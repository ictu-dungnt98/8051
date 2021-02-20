#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <string.h>

#include "IRsend.h"

#define NUMBER_KEY 5
#define F_IR 38 /* Tan so hoat dong cua IR */

#define _BUG_(...)                                        \
    do {                                                  \
        Serial.printf("%u\t  %s\t:", __LINE__, __FILE__); \
        Serial.println(__VA_ARGS__);                      \
    } while (0);
#define _BUGF_(...)                                       \
    do {                                                  \
        Serial.printf("%u\t  %s\t:", __LINE__, __FILE__); \
        Serial.printf(__VA_ARGS__);                       \
    } while (0);

const uint16_t kRecvPin = D1; /*Chân mắt thu hồng ngoại của esp8266  (pin 5) */
const uint8_t kIrLed = D2;    /*Chân mắt phát hồng ngoại của esp8266  (pin 5) */
unsigned long key_value = 0;

const int greenPin = 0;   // connected to GPIO pin 0 (D3 on a NodeMCU board).
const int yellowPin = 2;  // connected to GPIO pin 2 (D4 on a NodeMCU board).

IRrecv irrecv(kRecvPin);
IRsend irsend(kIrLed);

typedef enum { LEARN_IR_1 = 0, LEARN_IR_2, LEARN_IR_3, LEARN_IR_4, LEARN_IR_5, DEVICE_ACTIVE } state_machine_t;

state_machine_t state_machine;

decode_results results;
String name_key;

typedef struct {
    decode_results keyMap[NUMBER_KEY];
    String name_button[NUMBER_KEY];
    uint8_t number_key = 0;
} remote_t;

remote_t m_remote;

void setup()
{
    Serial.begin(115200);

    while (!Serial)  // Wait for the serial connection to be establised.
        delay(50);

    pinMode(greenPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);

    irsend.begin();
    irrecv.enableIRIn();  // Start the receiver

    memset(&m_remote, 0, sizeof(m_remote));
}

uint16_t *p_Raw = NULL;
uint8_t yes_no;
uint8_t cmd_control;

void loop()
{
    switch (state_machine) {
        case LEARN_IR_1: {
            Serial.println("Type name of button for learn or type EXIT for done learning task.");
            state_machine = LEARN_IR_2;
        } break;

        case LEARN_IR_2: {
            if (Serial.available()) {
                name_key = Serial.readStringUntil('\n');
                Serial.flush();
                Serial.println(name_key);

                name_key.trim();

                if (name_key == "exit" || name_key == "EXIT") {
                    Serial.println("Exit IR Application!");
                    state_machine = LEARN_IR_5;
                } else {
                    m_remote.name_button[m_remote.number_key] = name_key;
                    Serial.println("Push key on your remote.");
                    state_machine = LEARN_IR_3;
                }
            }

        } break;

        case LEARN_IR_3: {
            if (irrecv.decode(&results)) {
                serialPrintUint64(results.value, HEX);

                p_Raw = resultToRawArray(&results);
                irsend.sendRaw(p_Raw, strlen((char *)p_Raw), F_IR);
                _BUGF_(resultToSourceCode(&results).c_str());
                irrecv.resume();
                Serial.println("Do you want to save this button (1/0)?");
                state_machine = LEARN_IR_4;
            }
        } break;

        case LEARN_IR_4: {
            if (Serial.available()) {
                yes_no = Serial.read() - 48;
                Serial.flush();
                Serial.println(yes_no);

                if (yes_no) {
                    m_remote.keyMap[m_remote.number_key] = results;
                    m_remote.number_key++;
                    state_machine = LEARN_IR_1;
                    break;
                } else {
                    state_machine = LEARN_IR_3;
                    Serial.println("Push key on your remote to learn.");
                }
            }
        } break;

        case LEARN_IR_5: {
            Serial.println("Type your command you want to use:");
            state_machine = DEVICE_ACTIVE;
        } break;

        case DEVICE_ACTIVE: {
            if (Serial.available()) {
                cmd_control = Serial.read();
                cmd_control -= 48;
                Serial.println(cmd_control);

                if (cmd_control > m_remote.number_key) {
                    Serial.println("Command was not exist.");
                    break;
                }

                p_Raw = resultToRawArray(&m_remote.keyMap[cmd_control-1]);
                irsend.sendRaw(p_Raw, strlen((char *)p_Raw), F_IR);

                state_machine = LEARN_IR_5;
            }

        } break;

        default: {

        } break;
    }
}