//Classes/WiFiControl/WiFiControl.cpp
#include "WiFiControl.h"

WiFiControl::WiFiControl(IConfigManager& configMgr, std::unique_ptr<IWiFiStrategy> strategy)
  : _configMgr(configMgr), _strategy(std::move(strategy)), _lastConnection(millis()) {
  strcpy(_ssidBuffer, "N/A");
  strcpy(_ipBuffer, "0.0.0.0");
}

void WiFiControl::init() {
  _wifiInitialized = true;
  Serial.println("WiFi initialized");

  std::string ssid = _configMgr.GetWiFiSSID();
  std::string password = _configMgr.GetWiFiPass();
  if (!ssid.empty() && !password.empty()) {
    ConnectToAP(ssid, password);
  } else {
    WiFi.softAP(AP_WIFI_SSID, AP_WIFI_PASSWORD);
    if (_webServer) _webServer->StartDNS();
    strncpy(_ssidBuffer, AP_WIFI_SSID, sizeof(_ssidBuffer) - 1);
    _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
    strncpy(_ipBuffer, "192.168.4.1", sizeof(_ipBuffer) - 1);
    _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
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
    strncpy(_ssidBuffer, ssid.c_str(), sizeof(_ssidBuffer) - 1);
    _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
    strncpy(_ipBuffer, WiFi.localIP().toString().c_str(), sizeof(_ipBuffer) - 1);
    _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
    return true;
  } else {
    WiFi.softAP(AP_WIFI_SSID, AP_WIFI_PASSWORD);
    if (_webServer) _webServer->StartDNS();
    strncpy(_ssidBuffer, AP_WIFI_SSID, sizeof(_ssidBuffer) - 1);
    _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
    strncpy(_ipBuffer, "192.168.4.1", sizeof(_ipBuffer) - 1);
    _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
    return false;
  }
}

const char* WiFiControl::GetSSID() const {
  if (!_wifiInitialized) return "N/A";
  return _ssidBuffer;
}

const char* WiFiControl::GetIP() const {
  if (!_wifiInitialized) return "0.0.0.0";
  return _ipBuffer;
}

std::vector<NetworkInfo> WiFiControl::ScanNetworks() {
  if (!_wifiInitialized) {
    Serial.println("WiFi not initialized, skipping scan");
    return {};
  }
  std::vector<NetworkInfo> networks;
  int n = WiFi.scanComplete();
  if (n == -2) {
    WiFi.scanNetworks(true);
  } else if (n > 0) {
    for (int i = 0; i < n; ++i) {
      NetworkInfo network;
      network.ssid = WiFi.SSID(i).c_str();
      network.rssi = WiFi.RSSI(i);
      networks.push_back(network);
    }
    WiFi.scanDelete();
    if (WiFi.scanComplete() == -2) {
      WiFi.scanNetworks(true);
    }
  }
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