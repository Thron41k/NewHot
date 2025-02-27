#ifndef WEBSERVERCONTROL_H
#define WEBSERVERCONTROL_H

#include "Interfaces/IWebServer.h"
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <memory>
#include "Interfaces/IWiFiManager.h"
#include "Interfaces/IStatusProvider.h"
#include <Classes/DeviceStates/ConfigManager.h>

class WebServerControl : public IWebServer {
private:
  std::unique_ptr<AsyncWebServer> _webserver;
  std::unique_ptr<DNSServer> _dnsserver;
  IWiFiManager& _wifiManager;
  IConfigManager& _configMgr;
  const IStatusProvider& _statusProvider;
  bool _dnsState = false;
  bool _shouldReboot = false;

  void setupRoutes(); // Настройка маршрутов

public:
  WebServerControl(IWiFiManager& wifiManager, IConfigManager& configMgr, const IStatusProvider& statusProvider);
  void init(); // Инициализация сервера
  void StartDNS() override;
  void StopDNS() override;
  void Loop() override;
};

#endif