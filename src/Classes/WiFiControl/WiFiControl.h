// WiFiControl.h
#ifndef WIFICONTROL_H
#define WIFICONTROL_H

#include <WiFi.h>
#include "Interfaces/IWiFiManager.h"
#include "Classes/DeviceStates/Interfaces/IConfigManager.h"
#include "Classes/Helpers/Defines.h"
#include "NetworkInfo.h"
#include <memory>
#include "Interfaces/IWiFiStrategy.h"
#include "Interfaces/IWebServer.h"

class WiFiControl : public IWiFiManager
{
private:
    std::unique_ptr<IWiFiStrategy> _strategy;
    std::unique_ptr<IWebServer> _webServer;
    IConfigManager &_configMgr;
    uint32_t _lastConnection;
    bool _wifiInitialized = false;

public:
    WiFiControl(IConfigManager &configMgr, std::unique_ptr<IWiFiStrategy> strategy);
    void init();
    void setWebServer(std::unique_ptr<IWebServer> webServer);
    bool ConnectToAP(const std::string &ssid, const std::string &password) override;
    std::vector<NetworkInfo> ScanNetworks() override;
    bool IsConnected() const override { return WiFi.status() == WL_CONNECTED; }
    String GetSSID() const override { return WiFi.SSID(); }
    IPAddress GetIP() const override { return WiFi.localIP(); }
    void Loop() override;
    bool isWifiReady() const override; // Реализация метода интерфейса
};

#endif