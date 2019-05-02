#ifndef WIFIHELPER_H
#define WIFIHELPER_H

#include <Arduino.h>
#ifdef ARDUINO_ESP32_DEV
#include <WiFi.h>
#include <esp_wps.h>
#endif

class WiFiHelper
{
public:
    static void setup();
    static void reset();
    static bool connect();
    static bool connect(const char *ssid, const char *pwd);
    static void reconnect();
    static bool connected();
    static String getDeviceId();

private:
    static bool connectUsingWps();
    static bool connectUsingExistingCredentials();
    static bool hasCredentials();
    static String _deviceId;

#ifdef ARDUINO_ESP32_DEV
    static void WiFiEvent(WiFiEvent_t event, system_event_info_t info);
    static esp_wps_config_t _config;
    static bool _forceWPS;
#endif
};

#endif