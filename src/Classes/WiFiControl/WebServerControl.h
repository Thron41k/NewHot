//Classes/WiFiControl/WebServerControl.h
#ifndef WEBSERVERCONTROL_H
#define WEBSERVERCONTROL_H

#include "Interfaces/IWebServer.h"
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <memory>
#include "Interfaces/IWiFiManager.h"
#include "Interfaces/IStatusProvider.h"
#include <Classes/DeviceStates/ConfigManager.h>
#include "StatusProvider.h"

class WebServerControl : public IWebServer {
private:
  std::unique_ptr<AsyncWebServer> _webserver;
  std::unique_ptr<DNSServer> _dnsserver;
  IWiFiManager& _wifiManager;
  IConfigManager& _configMgr;
  StatusProvider _statusProvider;
  bool _dnsState = false;
  bool _shouldReboot = false;

  void setupRoutes(); // Настройка маршрутов

public:
  WebServerControl(IWiFiManager& wifiManager, IConfigManager& configMgr, StatusProvider statusProvider);
  void init(); // Инициализация сервера
  void StartDNS() override;
  void StopDNS() override;
  void Loop() override;
};

#endif