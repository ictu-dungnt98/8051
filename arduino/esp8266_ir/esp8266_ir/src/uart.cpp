#include "uart.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "smartIR.h"

char hc06_rx_queue[300];
uint16_t p_hc06_rx_data = 0;

void uart_init(void)
{
    Serial.begin(115200);
    while (!Serial);  // Wait for the serial connection to be establised.}
}

void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        Serial.print(F("DeserializeJson() failed\n"));
        Serial.println(error.c_str());
        return;
    }

    Serial.println(command);

    ir_control_AC(doc);
}
/* {"brand":12, "power":0, "temp":18, "mode":1, "fan":1, "swing":1, "model": 4}  */
void uart_handler(void)
{
    char ch;
    p_hc06_rx_data = 0;
    memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));

    if (Serial.available() > 0) {
        while (Serial.available() > 0) {
            ch = Serial.read();
            hc06_rx_queue[p_hc06_rx_data] = ch;
            p_hc06_rx_data++;

            delay(1);
        }

        Serial.print("recieved: ");
        Serial.println(hc06_rx_queue);

        /* Handler data recieved */
        handler_data(hc06_rx_queue);

        p_hc06_rx_data = 0;
        memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));
    }
}