#ifndef MATS_GLOBAL_H
#define MATS_GLOBAL_H

#include <Arduino.h>

// #define _DEBUGPRINT

#ifndef BUILTIN_LED
#ifdef ARDUINO_ESP32_DEV
#define BUILTIN_LED 2
#else
#define BUILTIN_LED LED_BUILTIN
#endif
#endif

#endif