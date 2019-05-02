#ifdef ARDUINO_ESP32_DEV
#include <Global.h>
#include "WiFiHelper.h"
#include <Arduino.h>
#include <DebugPrint.h>
#include <ErrorHelper.h>
#include <esp_wifi.h>

String WiFiHelper::_deviceId;

esp_wps_config_t WiFiHelper::_config = {
    WPS_TYPE_PBC,
    &g_wifi_default_wps_crypto_funcs,
    {
        "ESPRESSIF",
        "ESP32",
        "ESPRESSIF IOT",
        "ESP STATION",
    }};

bool WiFiHelper::_forceWPS = false;

void WiFiHelper::WiFiEvent(WiFiEvent_t event, system_event_info_t info)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_START:
        DebugPrintln("Station Mode Started");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        DebugPrintln("Connected to: " + String(WiFi.SSID()));
        DebugPrint("Got IP: ");
        DebugPrintln(WiFi.localIP());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        DebugPrintln("Disconnected from station, attempting reconnection");
        WiFi.reconnect();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        DebugPrintln("WPS Successfull, stopping WPS and connecting to: " + String(WiFi.SSID()));
        _forceWPS = false;
        esp_wifi_wps_disable();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        DebugPrintln("WPS Failed, retrying");
        esp_wifi_wps_disable();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        DebugPrintln("WPS Timedout, retrying");
        esp_wifi_wps_disable();
        break;
    default:
        break;
    }
}

void WiFiHelper::setup()
{
    WiFi.onEvent(WiFiEvent);
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
        if (!_forceWPS && hasCredentials())
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
            ErrorBlink("Wifi connect failed", 250, 10); // 5 sec
            connect();
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
    DebugPrintln("Connecting using WPS...");

    esp_err_t ret = esp_wifi_wps_enable(&_config);
    if (ret != ESP_OK)
    {
        DebugPrint("esp_wifi_wps_enable failed: ");
        DebugPrintln(esp_err_to_name(ret));
    }
    
    ret = esp_wifi_wps_start(1000 * 60);
    if (ret != ESP_OK)
    {
        DebugPrint("esp_wifi_wps_start failed: ");
        DebugPrintln(esp_err_to_name(ret));
    }

    DebugPrintln("Waiting for connect result...");
    WiFi.waitForConnectResult();

    if (WiFi.isConnected())
    {
        DebugPrintln("Connected using WPS");
    }
    else
    {
        DebugPrintln("Connect using WPS failed!");
    }

    return WiFi.isConnected();
}

bool WiFiHelper::connectUsingExistingCredentials()
{
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);

    String ssid(reinterpret_cast<const char *>(conf.sta.ssid));
    String pwd(reinterpret_cast<const char *>(conf.sta.password));

    return connect(ssid.c_str(), pwd.c_str());
}

bool WiFiHelper::connect(const char *ssid, const char *pwd)
{
    DebugPrintln("Connecting using credentials...");

    WiFi.begin(ssid, pwd);
    for (int i = 0; i < 10 && !WiFi.isConnected(); i++)
    {
        delay(500);
    }

    DebugPrintln("Waiting for connect result...");
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
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    String ssid(reinterpret_cast<const char *>(conf.sta.ssid));
    return ssid.length() > 0;
}

void WiFiHelper::reset()
{
    DebugPrintln("Resetting connection...");

    WiFi.disconnect(true);
    // WiFi.begin("", ""); // make a failed connection
    // WiFi.waitForConnectResult();

    _forceWPS = true;

    DebugPrintln("Connection resetted.");
}

#endif