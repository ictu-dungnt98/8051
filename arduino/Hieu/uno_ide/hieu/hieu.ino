#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include <ArduinoJson.h>

#define CMD_POST_DATA 0
#define CMD_RESET_DATA 1

/* Hardware Serial3 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
PZEM004Tv30 _pzem(10,11); /* connect to Tx,Rx of pzem004t */

void pzem004t_loop(void);

void water_sensor_setup(void);
void read_water_loop(void);
void post_inform_to_server(void);
void uno_handler_reset_data_new_month(void);

void setup() {
  Serial.begin(115200);
  water_sensor_setup();
}

void loop() {
    pzem004t_loop();
    read_water_loop();
    post_inform_to_server();
}

/************** PZEM004T **************/
float voltage;
float current;
float power;
float energy;
float frequency;
float pf;

void pzem_show(void)
{
    if (voltage != NAN) {
        Serial.print("Voltage: "); 
        Serial.print(voltage); 
        Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    if(current != NAN){
        Serial.print("Current: ");
        Serial.print(current);
        Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    if(power != NAN){
        Serial.print("Power: ");
        Serial.print(power);
        Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }

    if(energy != NAN){
        Serial.print("Energy: ");
        Serial.print(energy,3);
        Serial.println("kWh");
    } else {
        Serial.println("Error reading energy");
    }

    if(frequency != NAN){
        Serial.print("Frequency: ");
        Serial.print(frequency, 1);
        Serial.println("Hz");
    } else {
        Serial.println("Error reading frequency");
    }

    if(pf != NAN){
        Serial.print("PF: ");
        Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }
    Serial.println();
}

void pzem_reader(void)
{
//     if (digitalRead(OUTPUT_PIN)) {
        voltage = _pzem.voltage();
        current = _pzem.current();
        power = _pzem.power();
        energy = _pzem.energy();
        frequency = _pzem.frequency();
        pf = _pzem.pf();
//     }

//   pzem_show();
}

static uint32_t time_handler_pzem004t_before = 0;
void pzem004t_loop(void)
{
    if (millis() - time_handler_pzem004t_before > 1000) {
        time_handler_pzem004t_before = millis();
        pzem_reader();
    }
}

/*************** WARTER SENSOR ***************/

#define SENSOR 4

static uint32_t previousMillis = 0;
static float calibrationFactor = 4.5;
static uint8_t pulse1Sec = 0;
static float flowRate;

unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;

volatile uint8_t pulseCount;

void pulseCounter() {
    pulseCount++;
}

void water_sensor_setup(void)
{
    pinMode(SENSOR, INPUT_PULLUP);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;

    Serial.println("attachInterrupt");
    attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}

void read_water_loop(void)
{
    if (millis() - previousMillis > 5000) {
        pulse1Sec = pulseCount;
        pulseCount = 0;

        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = millis();

        // Divide the flow rate in litres/minute by 60 to determine how many litres have
        // passed through the sensor in this 1 second interval, then multiply by 1000 to
        // convert to millilitres.
        flowMilliLitres = (flowRate / 60) * 1000;
        flowLitres = (flowRate / 60);

        // Add the millilitres passed in this second to the cumulative total
        totalMilliLitres += flowMilliLitres;
        totalLitres += flowLitres;

        // Print the flow rate for this second in litres / minute
//        Serial.print("Flow rate: ");
//        Serial.print(float(flowRate));  // Print the integer part of the variable
//        Serial.print("L/min");
//        Serial.print("\t");  // Print tab space

//         Print the cumulative total of litres flowed since starting
//        Serial.print("Output Liquid Quantity: ");
//        Serial.print(totalMilliLitres);
//        Serial.print("mL / ");
//        Serial.print(totalLitres);
//        Serial.println("L");
    }
}


void post_inform_to_server(void)
{
    static uint32_t preMilis = 0;
    char buff[512];

    if (millis() - preMilis < 1000) /* 1 second */
        return;
    
    preMilis = millis();

    float tmpVal = (energy < 0) ? -energy : energy;

    int tmpInt1 = tmpVal;                  // Get the integer (678).
    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).

    memset(buff, 0, 512);
    sprintf(buff, "{\"cmd\":%d, \"energy\":\"%d.%03d\", \"water_mililites\": %d}",\
                    CMD_POST_DATA, tmpInt1, tmpInt2, totalMilliLitres);
    Serial.println(buff);
}

/******************** Handler for copmmunicate with esp8266 ********************/

char hc06_rx_queue[300];
uint16_t p_hc06_rx_data = 0;

void uart_init(void)
{
    Serial.begin(115200);
    while (!Serial);  // Wait for the serial connection to be establised.}
}

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

            delay(1); /*1ms */
        }

//        Serial.print("recieved: ");
//        Serial.println(hc06_rx_queue);

        /* Handler data recieved */
        handler_data(hc06_rx_queue);

        p_hc06_rx_data = 0;
        memset(hc06_rx_queue, 0, sizeof(hc06_rx_queue));
    }
}

void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        Serial.print(F("Decode fail\n"));
        Serial.println(error.c_str());
        return;
    }

    if ((uint8_t)doc["cmd"] == CMD_RESET_DATA) {
        uno_handler_reset_data_new_month();
    }
}

void uno_handler_reset_data_new_month(void)
{
    _pzem.resetEnergy();
    flowMilliLitres = 0;
    flowLitres = 0;
}
