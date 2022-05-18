#include "process_data.h"
#include <stdlib.h>
#include "motor.h"

extern int speed_motor_moving;

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

		case BACKWARD_LEFT: {
			backward_left();
		} break;

		case BACKWARD_RIGHT: {
			backward_right();
		} break;

		case SPEED_10: {
			set_speed(10);
		} break;

		case SPEED_20: {
			set_speed(20);
		} break;
		
		case SPEED_30: {
			set_speed(30);
		} break;

		case SPEED_40: {
			set_speed(40);
		} break;

		case SPEED_50: {
			set_speed(50);
		} break;

		case SPEED_60: {
			set_speed(60);
		} break;

		case SPEED_70: {
			set_speed(70);
		} break;

		case SPEED_80: {
			set_speed(80);
		} break;

		case SPEED_90: {
			set_speed(90);
		} break;

		case SPEED_100: {
			set_speed(100);
		} break;

		default:
			break;
    }
}


void process_data_by_byte(char cmd_type)
{
    if (cmd_type == 0) {
        return;
    }

    switch (cmd_type) {
		case TURN_LEFT: {
		} break;

		case TURN_RIGHT: {
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

		case BACKWARD_LEFT: {
			backward_left();
		} break;

		case BACKWARD_RIGHT: {
			backward_right();
		} break;

		case SPEED_10: {
			set_speed(10);
		} break;

		case SPEED_20: {
			set_speed(20);
		} break;
		
		case SPEED_30: {
			set_speed(30);
		} break;

		case SPEED_40: {
			set_speed(40);
		} break;

		case SPEED_50: {
			set_speed(50);
		} break;

		case SPEED_60: {
			set_speed(60);
		} break;

		case SPEED_70: {
			set_speed(70);
		} break;

		case SPEED_80: {
			set_speed(80);
		} break;

		case SPEED_90: {
			set_speed(90);
		} break;

		case SPEED_100: {
			set_speed(100);
		} break;

		default:
			break;
    }
}