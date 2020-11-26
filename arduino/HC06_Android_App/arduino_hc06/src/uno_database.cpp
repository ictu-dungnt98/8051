#include "uno_database.h"
#include "led_button.h"

device_info_t m_device;

void os_update_current_state_switch(hc06_ctrl_t state)
{
	switch(state) {
		case CH1_ON:
			m_device.m_state_out1 = CH1_ON;
			break;
		case CH1_OFF:
			m_device.m_state_out1 = CH1_OFF;
			break;
		case CH2_ON:
			m_device.m_state_out2 = CH2_ON;
			break;
		case CH2_OFF:
			m_device.m_state_out2 = CH2_OFF;
			break;
		case CH3_ON:
			m_device.m_state_out3 = CH3_ON;
			break;
		case CH3_OFF:
			m_device.m_state_out3 = CH3_OFF;
			break;
		default:
			break;
	}
}

void uno_get_device_infor_jsonform(char *buff, uint16_t len)
{
    memset(buff, 0, len);
    sprintf(buff, \
"{\"cmd_type\":3, \"state\":[%d, %d, %d], \"time_on\":[%d, %d, %d], \"time_off\":[%d, %d, %d]}\n", \
            digitalRead(LED1_PIN), digitalRead(LED2_PIN), digitalRead(LED3_PIN), \
            0, 0 ,0, \
            0, 0, 0);
}