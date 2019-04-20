#ifndef ERRORHELPER_H
#define ERRORHELPER_H

void ErrorBlinkLoop(int pin, int on, int off, int freq = 1000);
void ErrorBlinkLoop(char *msg, int pin, int on, int off, int freq = 1000);
void ErrorBlinkLoop(char *msg, int freq = 1000);
void ErrorBlink(char *msg, int freq, int times);

#endif