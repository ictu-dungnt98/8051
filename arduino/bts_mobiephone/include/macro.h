#pragma once

#include <Arduino.h>
extern "C" {
#include <c_types.h>
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define _BUG_(...)                                        \
    do {                                                  \
        Serial.println(__VA_ARGS__);                      \
    } while (0);

#define _BUGF_(...)                                       \
    do {                                                  \
        Serial.printf(__VA_ARGS__);                       \
    } while (0);
