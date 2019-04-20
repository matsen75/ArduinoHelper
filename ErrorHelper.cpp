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
    pinMode(LED_BUILTIN, OUTPUT);
    BlinkLoop(LED_BUILTIN, LOW, HIGH, freq, 0xFFFF);
}

void ErrorBlink(char *msg, int freq, int times)
{
    Serial.println(msg);
    pinMode(LED_BUILTIN, OUTPUT);
    BlinkLoop(LED_BUILTIN, LOW, HIGH, freq, times);
}