#include "handler_data.h"
#include <stdlib.h>
#include "m_typedef.h"
#include "led_button.h"
#include "uno_database.h"
#include "rtc.h"

extern struct tm m_time_alarm_on;
extern m_alarm_t m_time_alarm[MAX_CMD_ALARM];
extern uint8_t alarm_is_set;

/* {"cmd_type":0; "cmd":y} */
static void uno_handler_control_io(JsonDocument &_doc)
{
    uint8_t m_cmd = _doc["cmd"];
    // Serial.print("uno_handler_control_io!\n");

    control_device(m_cmd);
}

/* {cmd_type:1, "cmd":1, "hour":10, "minutes":10, "second":0} */
static void uno_handler_set_alarm(JsonDocument &_doc)
{
    if (alarm_is_set >= MAX_CMD_ALARM) {
        return;
    }

    // Serial.print("uno_handler_set_alarm!\n");

    char respond[256];
    uint8_t cmd = _doc["cmd"];

    /* Do set time alarm here */
    m_time_alarm[alarm_is_set].m_time.tm_hour = _doc["hour"];
    m_time_alarm[alarm_is_set].m_time.tm_min = _doc["minutes"];
    m_time_alarm[alarm_is_set].m_time.tm_sec = _doc["second"];
    m_time_alarm[alarm_is_set].m_cmd = cmd;

    if (alarm_is_set < MAX_CMD_ALARM) {
        Serial.print("alarm_is_set!");
        alarm_is_set ++;
    }

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":%d, \"res\":OK}\n", SET_ALARM);
    uno_respond_app(respond);
}

/* {cmd_type:2, "cmd":x, "day":11, "month":11, "year":2020, "hour":10, "minutes":10, "second":0} */
void uno_handler_remove_alarm(JsonDocument &_doc)
{
    char respond[256];

    /* clear alarm */
    alarm_is_set = 0;
    memset(&m_time_alarm, 0, sizeof(m_time_alarm));

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":%d, \"res\":OK}\n", RESET_ALARM);
    uno_respond_app(respond);
}

/* {"cmd_type":3} */
void uno_handler_query_info(void)
{
    report_current_state();
}

/* {"cmd_type":4, "dev":0} */
static void uno_handler_query_time_active_one_day(JsonDocument &_doc)
{
    uint8_t dev = _doc["dev"];

    uno_get_time_active_on_day(dev);
}

/* {"cmd_type":5} */
static void uno_handler_query_time_active_one_week()
{
    uno_get_time_active_in_week();
}

/* {"cmd_type":6} */
static void uno_handler_query_time_active_one_month()
{
    uno_get_time_active_in_month();
}

/* {"cmd_type":x; "cmd":y} */
void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        Serial.print(F("Dungnt98 deserializeJson() failed\n"));
        return;
    }

    uint8_t cmd_type = (uint8_t)doc["cmd_type"];
    Serial.print("cmd_type: ");
    Serial.println(cmd_type);

    switch (cmd_type) {
    case CONTROL_IO: {
        uno_handler_control_io(doc);
    } break;

    case SET_ALARM: {
        uno_handler_set_alarm(doc);
    } break;

    case RESET_ALARM: {
        uno_handler_remove_alarm(doc);
    } break;

    case QUERY_INFOM: {
        uno_handler_query_info();

    } break;

    case QUERY_TIME_DAY: {
        uno_handler_query_time_active_one_day(doc);

    } break;

    case QUERY_TIME_WEEK: {
        uno_handler_query_time_active_one_week();

    } break;

    case QUERY_TIME_MONTH: {
        uno_handler_query_time_active_one_month();

    } break;

    default:
        break;
    }
}