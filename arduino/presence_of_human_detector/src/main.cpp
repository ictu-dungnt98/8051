#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string.h>

#include "led.h"
#include "moving_average_filter.h"

#define NUMBER_RSSI 20
#define NUMBER_MAF 20

#define TIME_DELAY  2000

// const char* ssid = "Cua Nha Tao";
// const char* password = "0985329909";


const char* ssid = "Hunonic";
const char* password = "66668888";

int system_state = 0;
int raw_rssi1[NUMBER_RSSI];
int raw_rssi2[NUMBER_RSSI];
float smoothed_rssi1[NUMBER_RSSI];
float smoothed_rssi2[NUMBER_RSSI];
float mean1 = 0;
float variance1 = 0;
float mean2 = 0;
float variance2 = 0;

float mean = 0;
float variance = 0;
float mean_deviation = 0;
float variance_deviation = 0;
moving_average_t* sensor_av;

int current_rssi = 0;
float smoothed_rssi = 0;
int sum = 0;

void wifi_connect(void)
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("connect fail");
    delay(10);
  }
  // wait for first RSSI value to be returned (don't comment out).
  while (WiFi.RSSI() > 0) {
    delay(100);
  }

  Serial.println("wifi connected");
}

/** phase 1: Calibration
   Obtain n numbers of RSSI in a room without presence of human.
   The raw RSSI is smoothed using MAF.
*/
void do_calibration(void)
{
  static int calib_step = 0;

  do {
    printf("calib_step: %d\n", calib_step);
    switch (calib_step) {
      case 0: { /* obtain n numbers of RSSI */
          /* Fill MAF to get filter */
          for (int i = 0; i < NUMBER_MAF; i++) {
            current_rssi = -WiFi.RSSI();
            smoothed_rssi = movingAvg(sensor_av, current_rssi);
            printf("current_rssi/smoothed_rssi: %d / %.2f\n", current_rssi, smoothed_rssi);
            delay(TIME_DELAY);
          }

          /* obtain n numbers of RSSI */
          for (int i = 0; i < NUMBER_RSSI; i++) {
            current_rssi = -WiFi.RSSI();
            smoothed_rssi1[i] = movingAvg(sensor_av, current_rssi);
            printf("current_rssi/smoothed_rssi: %d / %.2f\n", current_rssi, smoothed_rssi1[i]);
            delay(TIME_DELAY);
          }

          /* obtain n numbers of RSSI again */
          for (int i = 0; i < NUMBER_RSSI; i++) {
            current_rssi = -WiFi.RSSI();
            smoothed_rssi2[i] = movingAvg(sensor_av, current_rssi);
            printf("current_rssi/smoothed_rssi: %d / %.2f\n", current_rssi, smoothed_rssi2[i]);
            delay(TIME_DELAY);
          }
          calib_step++;
        } break;

      case 1: { /* Calculate initial mean */
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

      case 2: { /* Calculate range of mean deviation */
          mean_deviation = mean2 - mean1;
          calib_step++;
        } break;

      case 3: { /* Calculate initial variance */
          /* variance 1 */
          for (int i = 0; i < NUMBER_RSSI; i++) {
            sum += (smoothed_rssi1[i] - mean1) * (smoothed_rssi1[i] - mean1);
          }
          double temp = (double)(sum / (NUMBER_RSSI - 1));
          variance1 = sqrt(temp);
          printf("sum1: %d, temp: %ld variance1: %2f\n", sum, temp, variance1);

          /* variance 2 */
          sum = 0;
          for (int i = 0; i < NUMBER_RSSI; i++) {
            sum += (smoothed_rssi2[i] - mean2) * (smoothed_rssi2[i] - mean2);
          }
          temp = (double)(sum / (NUMBER_RSSI - 1));
          variance2 = sqrt(temp);
          printf("sum2: %d, temp: %ld variance2: %2f\n", sum, temp, variance2);

          calib_step++;
        } break;

      case 4: { /* Calculate inital vardiance deviation */
          variance_deviation = abs(variance2 - variance1);
          calib_step++;
        } break;

      case 5: { /* Calibration was done, exit this function */
          calib_step = 0;
          mean = mean1;
          variance = variance1;
          printf("mean: %2f mean1: %2f variance: %2f variance1: %2f\n", mean, mean1, variance, variance1);
          return;
        } break;

      default:
        break;
    }
  } while (1);
}

/** phase 2: Main - Presence detection
   Read RSSI, smooth it, calculate mean and variance.
   Compare the mean and variance to the one obtain in calibration phase.
   ===> presence status -> control ON/OFF relay.
*/
void do_detection(void)
{
  static int detection_step = 0;

  Serial.printf("detection_step: %d\n", detection_step);
  switch (detection_step) {
    case 0: { /* obtain n RSSI */
        for (int i = 0; i < NUMBER_RSSI; i++) {
          current_rssi = -WiFi.RSSI();
          smoothed_rssi1[i] = movingAvg(sensor_av, current_rssi);
		  printf("current_rssi/smoothed_rssi: %d / %.2f\n", current_rssi, smoothed_rssi1[i]);
          delay(TIME_DELAY);
        }
        detection_step++;
      } break;

    case 1: { /* Calculate mean & variance */
        /* mean */
        for (int i = 0; i < NUMBER_RSSI; i++) {
          mean1 += smoothed_rssi1[i];
        }
        mean1 /= NUMBER_RSSI;

        /* variance */
        for (int i = 0; i < NUMBER_RSSI; i++) {
          sum += (smoothed_rssi1[i] - mean1) * (smoothed_rssi1[i] - mean1);
        }
        sum /= (NUMBER_RSSI - 1);
        variance1 = sqrt(sum);

        detection_step++;
      } break;

    case 2: { /* Compare mean and variance to initial calibrated parameters */
        printf("mean: %.2f mean1: %.2f mean_deviation: %.2f mean_diff: %.2f\n",
               mean, mean1, mean_deviation, mean > mean1 ? (mean - mean1) : (mean1 - mean));
        printf("variance1: %.2f variance: %.2f variance_deviation: %2f Vardiff: %.2f\n",
               variance1, variance, variance_deviation, 
			   variance > variance1 ? (variance - variance1) : (variance1 - variance));

        int check = false;
        if (((mean1 <= mean) && (abs(mean1) >= abs((int)(mean - mean_deviation)))) ||
            ((mean1 >= mean) && (abs(mean1) <= abs((int)(mean + mean_deviation)))))
        {
          if ((variance1 <= variance && variance1 >= (variance - variance_deviation)) ||
              (variance1 >= variance && variance1 <= (variance + variance_deviation)))
          {
            // k co nguoi
            check = true;
          }
        }

        if (check) {
          // k co nguoi
          led_off();
          Serial.println("khong co nguoi");
        } else {
          // co nguoi
          led_on();
          Serial.println("co nguoi");
        }

        detection_step++;
      } break;

    case 3: { /* reset */
        detection_step = 0;
      } break;

    default:
      break;
  }
}

/** configuration
    + wait user set wifi connect to and do
   calibration step.
    + jump to main step - detect person presence.
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
  Serial.println("running...");
  wifi_connect();
  led_init();
  sensor_av = allocate_moving_average(NUMBER_MAF);
}

void loop()
{
  presence_human_detector();
}