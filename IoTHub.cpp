#include <Global.h>
#include "IoTHub.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <DebugPrint.h>

// #define HOST "192.168.1.13"
// #define PORT 5001

#define HOST "192.168.1.150"
#define PORT 5000

void IoTHub::postMessage(JsonObject &json)
{
    HTTPClient http;

    if (!http.begin(HOST, PORT, "/api/v1/message"))
    {
        DebugPrintln("http.begin failed");
        return;
    }
    
    String output;
    json.printTo(output);

    http.addHeader("Content-Type", "application/json");
    int response = http.POST(output);
    
    DebugPrint("HTTP POST response: ");
    DebugPrintln(response);

    http.end();
}


String IoTHub::getLastMessage(String deviceId)
{
    HTTPClient http;

    String url = String("/api/v1/message?last=true&deviceId=") + deviceId;

    if (!http.begin(HOST, PORT, url))
    {
        DebugPrintln("http.begin failed");
        return String();
    }

    int httpCode = http.GET();

    String payload = httpCode == HTTP_CODE_OK ? http.getString() : String();
    DebugPrint("HTTP GET response: ");
    DebugPrintln(http.errorToString(httpCode));

    http.end();

    return payload;
}

String IoTHub::getLastMessageValue(String deviceId, String tag)
{
    HTTPClient http;

    String url = String("/api/v1/messageValue?last=true&deviceId=") + deviceId + String("&tag=") + tag;

    if (!http.begin(HOST, PORT, url))
    {
        DebugPrintln("http.begin failed");
        return String();
    }

    int httpCode = http.GET();

    String payload = httpCode == HTTP_CODE_OK ? http.getString() : String();
    DebugPrint("HTTP GET response: ");
    DebugPrint(httpCode);
    DebugPrint(", ");
    DebugPrintln(http.errorToString(httpCode));

    http.end();

    return payload;
}