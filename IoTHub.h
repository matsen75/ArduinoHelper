#ifndef IOTHUB_H
#define IOTHUB_H

#include <Arduino.h>
#include <ArduinoJson.h>

class IoTHub
{
  public:
    void postMessage(JsonObject &json);
    String getLastMessage(String deviceId);
    String getLastMessageValue(String deviceId, String tag);
};

#endif