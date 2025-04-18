//Classes/WiFiControl/WiFiControl.h
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

class WiFiControl : public IWiFiManager {
  private:
    std::unique_ptr<IWiFiStrategy> _strategy;
    std::unique_ptr<IWebServer> _webServer;
    IConfigManager& _configMgr;
    uint32_t _lastConnection;
    bool _wifiInitialized = false;
    char _ssidBuffer[33];
    char _ipBuffer[16];
  
  public:
    WiFiControl(IConfigManager& configMgr, std::unique_ptr<IWiFiStrategy> strategy);
    void init();
    void setWebServer(std::unique_ptr<IWebServer> webServer);
    bool ConnectToAP(const std::string& ssid, const std::string& password) override;
    std::vector<NetworkInfo> ScanNetworks() override;
    bool IsConnected() const override { return WiFi.status() == WL_CONNECTED; }
    const char* GetSSID() const override;
    const char* GetIP() const override;
    void Loop() override;
    bool isWifiReady() const override;
    void startScan() override {} // Оставляем пустым, так как не используется
    bool isScanComplete() const override { return false; } // Не используется
    std::vector<NetworkInfo> getScanResults() const override { return std::vector<NetworkInfo>(); } // Не используется
  };
#endif