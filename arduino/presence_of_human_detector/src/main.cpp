#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "led.h"
#include "moving_average_filter.h"

#define NUMBER_RSSI 50
#define NUMBER_MAF 20

const char* ssid = "Dungnt98";
const char* password = "Peppe123";

int system_state = 0;
int raw_rssi1[NUMBER_RSSI];
int raw_rssi2[NUMBER_RSSI];
int smoothed_rssi1[NUMBER_RSSI];
int smoothed_rssi2[NUMBER_RSSI];
int mean1 = 0;
int variance1 = 0;
int mean2 = 0;
int variance2 = 0;
int mean_deviation = 0;
int variance_deviation = 0;
moving_average_t* sensor_av;

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
	static int currentRSSI = 0;
	static int new_rssi_smoothed = 0;
	int sum = 0;
    do {
        switch (calib_step) {
            case 0: { /* obtain n numbers of RSSI */
				/* Fill MAF to get filter */
                for (int i = 0; i < NUMBER_MAF+1; i++) {
                    currentRSSI = WiFi.RSSI();
					new_rssi_smoothed = movingAvg(sensor_av, currentRSSI);
                }

				/* obtain n numbers of RSSI */
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    raw_rssi1[i] = WiFi.RSSI();
                }
				/* obtain n numbers of RSSI */
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    raw_rssi2[i] = WiFi.RSSI();
                }
                calib_step++;
            } break;

            case 1: { /* smooth sample of raw RSSI */
				/* smooth array 1 */
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    smoothed_rssi1[i] = movingAvg(sensor_av, raw_rssi1[i]);
                }
				/* smooth array 2 */
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    smoothed_rssi2[i] = movingAvg(sensor_av, raw_rssi2[i]);
                }
                calib_step++;
            } break;

            case 2: { /* Calculate initial mean */
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    mean1 += smoothed_rssi1[i];
                }
				mean1 /= NUMBER_RSSI;

				for (int i = 0; i < NUMBER_RSSI; i++) {
                    mean2 += smoothed_rssi2[i];
                }
				mean2 /= NUMBER_RSSI;
                calib_step++;
            } break;

            case 3: { /* Calculate range of mean deviation */
				mean_deviation = abs(mean2 - mean1);
                calib_step++;
            } break;

            case 4: { /* Calculate initial variance */
				/* variance 1 */
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    sum += (smoothed_rssi1[i] - mean1) * (smoothed_rssi1[i] - mean1);
                }
				sum /= (NUMBER_RSSI-1);
				variance1 = sqrt(sum);

				/* variance 2 */
				sum = 0;
				for (int i = 0; i < NUMBER_RSSI; i++) {
                    sum += (smoothed_rssi2[i] - mean2) * (smoothed_rssi2[i] - mean2);
                }
				sum /= (NUMBER_RSSI-1);
				variance1 = sqrt(sum);

                calib_step++;
            } break;

            case 5: { /* Calculate inital vardiance deviation */
				variance_deviation = abs(variance2 - variance1);
                calib_step++;
            } break;

			case 6: { /* Calibration was done, exit this function */
                calib_step = 0;
				return;
            } break;

            default:
                break;
        }
    } while (1);
}

/** phase 2: Main - Presence detection
 * Read RSSI, smooth it, calculate mean and variance.
 * Compare the mean and variance to the one obtain in calibration phase.
 * ===> presence status -> control ON/OFF relay.
 */
void do_detection(void) 
{
	static int detection_step = 0;
	static int currentRSSI = 0;
	static int new_rssi_smoothed = 0;

	switch (detection_step) {
		case 0: { /* obtain number of RSSI */
			currentRSSI = WiFi.RSSI();
			detection_step++;
		} break;

		case 1: { /* smooth raw RSSI */
			new_rssi_smoothed = movingAvg(sensor_av, currentRSSI);
			detection_step++;
		} break;

		case 2: { /* sample n reading of smoothed RSSI */
			detection_step++;
		} break;

		case 3: { /* Calculate mean & variance */
			detection_step++;
		} break;

		case 4: { /* Compare mean and variance to initial calibrated parameters */
			detection_step++;
		} break;

		case 5: { /* reset */
			detection_step = 0;
		} break;

		default:
			break;
	}
}

/** configuration
 * 	+ wait user set wifi connect to and do
 * calibration step.
 * 	+ jump to main step - detect person presence.
 */

void presence_human_detector(void)
{
    switch (system_state) {
        case 0: {
            do_calibration();
            system_state++;
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
    led_init();
    sensor_av = allocate_moving_average(NUMBER_RSSI);
}

void loop()
{
    presence_human_detector();
}
