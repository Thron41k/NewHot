#ifndef STATUSPROVIDER_H
#define STATUSPROVIDER_H

#include "Interfaces/IWiFiManager.h"
#include "Interfaces/IStatusProvider.h"

class StatusProvider : public IStatusProvider {
private:
  const IWiFiManager& _wifiManager;
public:
  StatusProvider(const IWiFiManager& wifiManager) : _wifiManager(wifiManager) {}
  String GetStatusJson() const override {
    String json = "{";
    json += "\"wifi_mode\":\"" + String(_wifiManager.IsConnected() ? "STA" : "AP") + "\",";
    json += "\"wifi_status\":\"" + String(_wifiManager.IsConnected() ? "Connected" : "Disconnected") + "\",";
    json += "\"ssid\":\"" + _wifiManager.GetSSID() + "\",";
    json += "\"ip\":\"" + _wifiManager.GetIP().toString() + "\"";
    json += "}";
    return json;
  }
};

#endif