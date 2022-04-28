#include "hc06.h"
#include "squeue.h"
#include "handler_data.h"

#define MAX_QUEUE_ITEM 16

SoftwareSerial HC06(12, 13); // RX, TX

/* Rx buffer */
static sq_t rx_queue;


void hc06_init(void)
{
    Serial.begin(9600);
    HC06.begin(9600);

	sq_init(&rx_queue, sizeof(msg_t), MAX_QUEUE_ITEM);
}

void hc06_read_data(void)
{
    char ch;
	msg_t temp_data;
	char hc06_rx_queue[256];
	uint16_t p_hc06_rx_data = 0;

    memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));
	memset(&temp_data, 0, sizeof(temp_data));

    if (HC06.available() > 0) {
        while (HC06.available() > 0) {
            ch = HC06.read();
            hc06_rx_queue[p_hc06_rx_data] = ch;
            p_hc06_rx_data ++;
            delay(1); /* Delay for wait data ready on serial port */
        }
		
        Serial.print("recieved: ");
        Serial.println(hc06_rx_queue);

		temp_data.len = p_hc06_rx_data;
		memcpy(temp_data.data, hc06_rx_queue, p_hc06_rx_data);

        sq_push(&rx_queue, &temp_data);
    }
}

void hc06_process_data(void)
{
	msg_t rx_data;
	sq_pop(&rx_queue, &rx_data);
    if (rx_data.len) {
        process_data(rx_data.data);
    }
}
