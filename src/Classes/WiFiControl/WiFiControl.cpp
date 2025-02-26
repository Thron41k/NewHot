#include "WiFiControl.h"

WiFiControl::WiFiControl(IConfigManager &configMgr, std::unique_ptr<IWiFiStrategy> strategy, std::unique_ptr<IWebServer> webServer) : _configMgr(configMgr)
{
  Serial.println("WiFiControl initialized -3");
  _strategy = std::move(strategy);
  Serial.println("WiFiControl initialized -2");
  _webServer = std::move(webServer);
  Serial.println("WiFiControl initialized -1");
  _lastConnection = millis();
  Serial.println("WiFiControl initialized 0");
  ConnectToAP(_configMgr.GetWiFiSSID(), _configMgr.GetWiFiPass());
  Serial.println("WiFiControl initialized 1");
}

bool WiFiControl::ConnectToAP(const std::string &ssid, const std::string &password)
{
  if (_strategy->Connect(ssid, password))
  {
    _configMgr.SetWiFiCredentials(ssid.c_str(), password.c_str());
    _webServer->StopDNS();
    return true;
  }
  else
  {
    _strategy->StartAP();
    _webServer->StartDNS();
    return false;
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

void WiFiControl::Loop()
{
  _webServer->Loop();
  if (millis() - _lastConnection >= WIFI_RECONNECTION_TIMEOUT && WiFi.getMode() == WIFI_AP)
  {
    _lastConnection = millis();
    ConnectToAP(_configMgr.GetWiFiSSID(), _configMgr.GetWiFiPass());
  }
}