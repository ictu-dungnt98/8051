#ifndef _SMARTIR_H_
#define _SMARTIR_H_

#include <ArduinoJson.h>

#define F_IR 38 /* Tan so hoat dong cua IR */

typedef void (*brandToCallBack_t)(JsonDocument &root);

void smartir_setup_io(void);
void smartir_loop_handler(void);
void send_command_learnt(String name);

#endif /* _SMARTIR_H_ */