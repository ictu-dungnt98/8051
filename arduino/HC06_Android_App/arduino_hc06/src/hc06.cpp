#include "hc06.h"
#include "m_typedef.h"
#include "handler_data.h"

char hc06_rx_queue[300];
uint16_t p_hc06_rx_data = 0;

SoftwareSerial mySerial(10, 11); // RX, TX

void hc06_init(void)
{
    Serial.begin(9600);
    // mySerial.begin(115200);
}

void hc06_hander(void)
{
    char ch;

    p_hc06_rx_data = 0;
    memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));

    if (Serial.available() > 0) {
        while (Serial.available() > 0) {
            ch = Serial.read();
            hc06_rx_queue[p_hc06_rx_data] = ch;
            p_hc06_rx_data ++;

            // delay(1); /* Delay for wait data ready on serial port */
        }

        // Serial.print("Dungnt98 done read serial data\n");
        Serial.println(hc06_rx_queue);

        /* Handler data recieved */
        handler_data(hc06_rx_queue);
    }
}