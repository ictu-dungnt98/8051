#include "uno_database.h"

device_info_t m_device;

void uno_get_device_infor_jsonform(char *buff, uint16_t len)
{
    memset(buff, 0, len);
    sprintf(buff,
                "{\"cmd_type\":%d, \"time_on_out1\":%d, \"time_off_out1\":%d, \
\"time_on_out2\":%d, \"time_off_out2\":%d, \
\"time_on_out3\":%d, \"time_off_out3\":%d, \
\"state_out1\":%d, \"state_out2\":%d, \"state_out3\":%d}", \
                QUERY_INFOM, m_device.time_on_out1, m_device.time_off_out1,
                m_device.time_on_out2, m_device.time_off_out2,
                m_device.time_on_out3, m_device.time_off_out3,
                m_device.m_state_out1, m_device.m_state_out2, m_device.m_state_out3);
}