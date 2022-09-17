#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "PubSubClient.h"
#include "led_button.h"
#include "handler_mqtt.h"
#include "database.h"

#define NUMBER_RSSI	20

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

int raw_rssi[NUMBER_RSSI];

void wifi_connect(void)
{
	WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }
	// wait for first RSSI value to be returned (don't comment out).
    while (WiFi.RSSI() > 0) {
        delay(100);
    }
}

/** phase 1: Calibration
 * Obtain n numbers of RSSI in a room without presence of human.
 * The raw RSSI is smoothed using MAF.
 */
void do_calibration(void)
{
	static int calib_step = 0;

	switch(calib_step) {
		case 0: { /* obtain n numbers of RSSI */
			memset(raw_rssi, 0, sizeof(raw_rssi));
			for (int i = 0; i < NUMBER_RSSI; i++) {
				raw_rssi[i] = WiFi.RSSI();
			}
			calib_step ++;
		} break;

		case 1: { /* smooth sample of raw RSSI */
			calib_step ++;
		} break;

		case 2: { /* Calculate initial mean */
			calib_step ++;
		} break;

		case 3: { /* Calculate initial mean */
			calib_step ++;
		} break;

		case 4: { /* Calculate range of mean deviation */
			calib_step ++;
		} break;

		case 5: { /* Calculate initial variance */
			calib_step ++;
		} break;

		
		case 6: { /* Calculate inital vảiance deviation */
			calib_step ++;
		} break;

		default:
			break;
	}
}

/** phase 2: Main - Presence detection
 * Read RSSI, smooth it, calculate mean and variance.
 * Compare the mean and variance to the one obtain in calibration phase.
 * ===> presence status -> control ON/OFF relay.
 */
void do_detection(void)
{
	int currentRSSI = WiFi.RSSI();
}

/** configuration 
 * 1. Initialize wifi feature.
 * 2. read eeprom to determine whether device was configured or not.
 * 	+ if device was not configured, wait user set wifi connect to and do calibration step.
 * 	+ if device was configured, jump to main step - detect person presence.
 */

void presence_human_detector(void)
{
	switch (m_device.device_was_calibrated) {
		case 0: {
			do_calibration();
		    sync_database();
			break;
		}
		case 1: {
			do_detection();
			break;
		}
		default:
			break;
	}
}

void setup()
{
    Serial.begin(115200);
    wifi_connect();
    eeprom_init();
    led_button_init();
    mqtt_init();
}

void loop()
{
	presence_human_detector();
    mqtt_handler();
    button_loop();
}
