#include "WiFiControl.h"

WiFiControl::WiFiControl(IConfigManager& configMgr, std::unique_ptr<IWiFiStrategy> strategy)
  : _configMgr(configMgr), _strategy(std::move(strategy)), _lastConnection(millis()) {
}

void WiFiControl::init() {
  WiFi.mode(WIFI_STA);
  _wifiInitialized = true;
  Serial.println("WiFi initialized");

  std::string ssid = _configMgr.GetWiFiSSID();
  std::string password = _configMgr.GetWiFiPass();
  if (!ssid.empty() && !password.empty()) {
    ConnectToAP(ssid, password);
  } else {
    _strategy->StartAP();
    if (_webServer) _webServer->StartDNS();
  }
}

void WiFiControl::setWebServer(std::unique_ptr<IWebServer> webServer) {
  _webServer = std::move(webServer);
  if (_webServer && _wifiInitialized && WiFi.getMode() == WIFI_AP) {
    _webServer->StartDNS();
  }
}

bool WiFiControl::ConnectToAP(const std::string& ssid, const std::string& password) {
  if (!_wifiInitialized) {
    Serial.println("WiFi not initialized, skipping connect");
    return false;
  }
  if (_strategy->Connect(ssid, password)) {
    _configMgr.SetWiFiCredentials(ssid.c_str(), password.c_str());
    if (_webServer) _webServer->StopDNS();
    return true;
  } else {
    _strategy->StartAP();
    if (_webServer) _webServer->StartDNS();
    return false;
  }
}

std::vector<NetworkInfo> WiFiControl::ScanNetworks() {
  if (!_wifiInitialized) {
    Serial.println("WiFi not initialized, skipping scan");
    return {};
  }
  std::vector<NetworkInfo> networks;
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks > 0) {
    for (int i = 0; i < numNetworks; ++i) {
      NetworkInfo network;
      network.ssid = WiFi.SSID(i).c_str();
      network.rssi = WiFi.RSSI(i);
      networks.push_back(network);
    }
  }
  WiFi.scanDelete();
  return networks;
}

void WiFiControl::Loop() {
  if (_webServer && _wifiInitialized) _webServer->Loop();
  if (_wifiInitialized && millis() - _lastConnection >= WIFI_RECONNECTION_TIMEOUT && WiFi.getMode() == WIFI_AP) {
    _lastConnection = millis();
    ConnectToAP(_configMgr.GetWiFiSSID(), _configMgr.GetWiFiPass());
  }
}

bool WiFiControl::isWifiReady() const {
  return _wifiInitialized;
}