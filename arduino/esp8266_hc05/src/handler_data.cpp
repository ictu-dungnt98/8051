#include "handler_data.h"
#include <stdlib.h>
#include "motor.h"


void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

	uint8_t cmd_type = atoi(command);

    Serial.println(command);

    switch (cmd_type) {
    case TURN_LEFT: {
        go_left();
    } break;

	case TURN_RIGHT: {
		go_right();
	} break;

	case TURN_FORWARD: {
		go_forward();
	} break;

	case TURN_BACKWARD: {
		go_backward();
	} break;

	case TURN_STOP: {
		stop();
	} break;
   
    default:
        Serial.println("Cmd_type was not match!");
        break;
    }
}