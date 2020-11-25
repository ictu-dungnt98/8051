#include "handler_data.h"
#include <ArduinoJson.h>

#include "m_typedef.h"
#include "led_button.h"
#include "uno_database.h"

/* {"cmd_type":x; "cmd":y} */
static void uno_handler_control_io(JsonDocument &_doc)
{
    uint8_t m_cmd = _doc["cmd"];

    switch (m_cmd)
    {
    case CH1_ON: {
        gpio_on(LED1_PIN);
    } break;

    case CH1_OFF: {
        gpio_off(LED1_PIN);
    } break;

    case CH2_ON: {
        gpio_on(LED2_PIN);
    } break;

    case CH2_OFF: {
        gpio_off(LED2_PIN);
    } break;

    case CH3_ON: {
        gpio_on(LED3_PIN);
    } break;

    case CH3_OFF: {
        gpio_off(LED3_PIN);
    } break;
    
    default: {
        Serial.print("Control invalid pin\n");
    } break;
    }
}

/* {"cmd_type":x; "t_on":y, "t_off":z} */
static void uno_handler_set_alarm(JsonDocument &_doc)
{
    uint8_t t_on = _doc["t_on"];
    uint8_t t_off = _doc["t_off"];
    char respond[256];

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":1, \"t_on\":%d, \"t_off\":%d}\n", t_on, t_off);
    Serial.print(respond);
    /* Do Alarm here */
}

/* {"cmd_type":2} */
static void uno_handler_query_info(void)
{
    char m_device_info[512];

    /* Send device infor to app */
    uno_get_device_infor_jsonform(m_device_info, sizeof(m_device_info));
    Serial.print(m_device_info);
}

/* {"cmd_type":x; "cmd":y} */
void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        Serial.print(F("Dungnt98 deserializeJson() failed\n"));
        return;
    }

    uint8_t cmd_type = doc["cmd_type"];

    switch (cmd_type) {
    case CONTROL_IO: {
        uno_handler_control_io(doc);
    } break;

    case SET_ALARM: {
        uno_handler_set_alarm(doc);
    } break;

    case QUERY_INFOM: {
        uno_handler_query_info();

    } break;

    default:
        break;
    }
}