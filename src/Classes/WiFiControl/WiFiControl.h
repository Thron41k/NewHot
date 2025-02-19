#ifndef WIFICONTROL_H
#define WIFICONTROL_H

#include <Classes/DeviceStates/DeviceStates.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <WiFi.h>
#include "Classes/Helpers/Defines.h"
#include "NetworkInfo.h"

class WiFiControl;

class WebServerControl
{
private:
    std::unique_ptr<AsyncWebServer> _webserver;
    std::unique_ptr<DNSServer> _dnsserver;
    WiFiControl &_wifiControl;
    bool _dns_state = false;
    bool shouldReboot = false;
public:
    WebServerControl(WiFiControl &w);
    void Loop();
    ~WebServerControl() = default;
    void StartDNS();
    void StopDNS();
};

class WiFiControl
{
private:
    std::unique_ptr<WebServerControl> _webserverControl;
    const DeviceStates &_deviceStates;
    void StartAP();
    uint32_t _last_connection;
public:
    WiFiControl(const DeviceStates &deviceStates);
    ~WiFiControl() = default;

    std::vector<NetworkInfo> ScanNetworks();
    bool ConnectToAP(const std::string &ssid, const std::string &password);
    void Loop();
    const DeviceStates* GetDeviceStates() const { return &_deviceStates; }
};

#endif