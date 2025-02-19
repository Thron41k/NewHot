#include "WiFiControl.h"

WiFiControl::WiFiControl(const DeviceStates &deviceStates) : _deviceStates(deviceStates)
{
    _webserverControl = std::make_unique<WebServerControl>(this);
    ConnectToAP(_deviceStates.GetWiFiSSID(), _deviceStates.GetWiFiPass());
}

void WiFiControl::StartAP()
{
    WiFi.softAP(AP_WIFI_SSID, AP_WIFI_PASSWORD);
    _webserverControl->StartDNS();
    _last_connection = millis();
}

bool WiFiControl::ConnectToAP(const std::string &ssid, const std::string &password) 
{
    WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_CONNECTION_ATTEMPTS)
    {
        delay(500);
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        _deviceStates.SetWiFiCredentials(ssid.c_str(), password.c_str());
        _webserverControl->StopDNS();
        return true;
    }
    else
    {
        StartAP();
    }
    return false;
}

void WiFiControl::Loop()
{
    _webserverControl->Loop();
    if (millis() - _last_connection >= WIFI_RECONNECTION_TIMEOUT && WiFi.getMode() == WIFI_AP)
    {
        _last_connection = millis();
        ConnectToAP(_deviceStates.GetWiFiSSID(), _deviceStates.GetWiFiPass());
    }
}

std::vector<NetworkInfo> WiFiControl::ScanNetworks()
{
    std::vector<NetworkInfo> networks;
    int numNetworks = WiFi.scanNetworks();
    if (numNetworks > 0)
    {
        for (int i = 0; i < numNetworks; ++i)
        {
            NetworkInfo network;
            network.ssid = WiFi.SSID(i).c_str();
            network.rssi = WiFi.RSSI(i);
            networks.push_back(network);
        }
    }
    WiFi.scanDelete();
    return networks;
}