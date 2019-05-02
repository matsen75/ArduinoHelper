#ifndef ARDUINO_ESP32_DEV
#include <Global.h>
#include "WiFiHelper.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DebugPrint.h>
#include <ErrorHelper.h>

String WiFiHelper::_deviceId;

void WiFiHelper::setup()
{
    WiFi.mode(WIFI_STA);
}

bool WiFiHelper::connect()
{
    bool success = false;

    if (WiFi.isConnected())
    {
        DebugPrintln("Already connected");
        success = true;
    }
    else
    {
        if (hasCredentials())
        {
            success = connectUsingExistingCredentials();
        }
        else
        {
            success = connectUsingWps();
        }
    }

    if (success)
    {
        DebugPrintln("Connected!");
    }
    else
    {
        DebugPrint("Connect failed, status: ");
        DebugPrintln(WiFi.status());
    }

    return success;
}

bool WiFiHelper::connected()
{
    return WiFi.isConnected();
}

void WiFiHelper::reconnect()
{
    if (!WiFi.isConnected())
    {
        connect();
        while (!WiFi.isConnected())
        {
            ErrorBlink("Wifi connect failed", 250, 20); // 10 sec
            
            String ssid(WiFi.SSID());
            String pwd(WiFi.psk());
            
            reset();
            
            if (ssid.length() > 0)
            {
                connect(ssid.c_str(), pwd.c_str());
            }
            else
            {
                connect();
            }
            
        }
    }
}

String WiFiHelper::getDeviceId()
{
    if (_deviceId.length() == 0)
    {
        _deviceId = String("ESP_" + WiFi.macAddress());
        _deviceId.replace(":", "");
    }
    return _deviceId;
}

bool WiFiHelper::connectUsingWps()
{
    bool success = WiFi.beginWPSConfig();

    WiFi.waitForConnectResult();

    if (success)
    {
        DebugPrintln("Connected using WPA");
    }
    else
    {
        DebugPrintln("Connect using WPA failed!");
    }

    return success;
}

bool WiFiHelper::connectUsingExistingCredentials()
{
    return connect(WiFi.SSID().c_str(), WiFi.psk().c_str());
}

bool WiFiHelper::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);
    for (int i = 0; i < 10 && !WiFi.isConnected(); i++)
    {
        delay(500);
    }

    WiFi.waitForConnectResult();

    if (WiFi.isConnected())
    {
        DebugPrintln("Connected using existing credentials");
    }
    else
    {
        DebugPrintln("Connect using credentials failed!");
    }

    return WiFi.isConnected();
}

bool WiFiHelper::hasCredentials()
{
    return WiFi.SSID().length() > 0;
}

void WiFiHelper::reset()
{
    WiFi.disconnect();
    WiFi.begin("", ""); // make a failed connection
    while (WiFi.status() == WL_DISCONNECTED)
    {
        delay(500);
    }
}

#endif