#include "process_data.h"
#include <stdlib.h>
#include "motor.h"

void process_data(char* command)
{
    if (command == NULL) {
        return;
    }

	uint8_t cmd_type = command[0];
    Serial.println(cmd_type);

    switch (cmd_type) {
		case TURN_LEFT: {
			turn_left();
		} break;

		case TURN_RIGHT: {
			turn_right();
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

		case FORWARD_LEFT: {
			forward_left();
		} break;

		case FORWARD_RIGHT: {
			forward_right();
		} break;

		default:
			break;
    }
}