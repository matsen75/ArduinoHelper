#ifndef WIFIHELPER_H
#define WIFIHELPER_H

#include <Arduino.h>

class WiFiHelper
{
public:
    static void setup();
    static void reset();
    static bool connect();
    static bool connect(const char* ssid, const char* pwd);
    static void reconnect();
    static bool connected();
    static String getDeviceId();
    
private:
    static bool connectUsingWps();
    static bool connectUsingExistingCredentials();
    static bool hasCredentials();
    static String _deviceId;
};

#endif