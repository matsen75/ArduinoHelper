#include <Global.h>
#include "ErrorHelper.h"
#include <Arduino.h>

void BlinkLoop(int pin, int on, int off, int freq, uint16_t times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(pin, on);
        delay(freq);
        digitalWrite(pin, off);
        delay(freq);
    }
}

void ErrorBlinkLoop(int pin, int on, int off, int freq)
{
    BlinkLoop(pin, on, off, freq, 0xFFFF);
}

void ErrorBlinkLoop(char *msg, int pin, int on, int off, int freq)
{
    Serial.println(msg);
    BlinkLoop(pin, on, off, freq, 0xFFFF);
}

void ErrorBlinkLoop(char *msg, int freq)
{
    Serial.println(msg);
    pinMode(BUILTIN_LED, OUTPUT);
#ifdef ARDUINO_ESP32_DEV
    BlinkLoop(BUILTIN_LED, HIGH, LOW, freq, 0xFFFF);
#else
    BlinkLoop(BUILTIN_LED, LOW, HIGH, freq, 0xFFFF);
#endif
}

void ErrorBlink(char *msg, int freq, int times)
{
    Serial.println(msg);
    pinMode(BUILTIN_LED, OUTPUT);
#ifdef ARDUINO_ESP32_DEV
    BlinkLoop(BUILTIN_LED, HIGH, LOW, freq, times);
#else
    BlinkLoop(BUILTIN_LED, LOW, HIGH, freq, times);
#endif
}