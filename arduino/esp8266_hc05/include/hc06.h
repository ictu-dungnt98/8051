#ifndef _HC06_H_
#define _HC06_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

void hc06_init(void);

void hc06_read_data(void);
void hc06_process_data(void);

#endif