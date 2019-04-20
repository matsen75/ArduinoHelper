#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#ifdef _DEBUGPRINT
    #define DebugBegin(rate) Serial.begin(rate)
    #define DebugPrint(text) Serial.print(text)
    #define DebugPrintHEX(text) Serial.print(text, HEX)
    #define DebugPrintln(text) Serial.println(text)
    #define DebugPrintlnHEX(text) Serial.println(text, HEX)
    #define DebugPrettyPrintToSerial(jobj) jobj.prettyPrintTo(Serial)
#else
    #define DebugBegin(rate)
    #define DebugPrint(text)
    #define DebugPrintHEX(text)
    #define DebugPrintln(text)
    #define DebugPrintlnHEX(text)
    #define DebugPrettyPrintToSerial(jobj)
#endif

#endif