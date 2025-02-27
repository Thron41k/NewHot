
#ifndef WIFICONTROLSTASTRATEGY_H
#define WIFICONTROLSTASTRATEGY_H

#include "Interfaces/IWiFiStrategy.h"
#include <WiFi.h>
#include <Classes/Helpers/Defines.h>

class WiFiSTAStrategy : public IWiFiStrategy
{
public:
  bool Connect(const std::string &ssid, const std::string &password) override
  {
    Serial.println("Connecting to WiFi...");
    //WiFi.softAPdisconnect();
    //delay(1000);
   // WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_CONNECTION_ATTEMPTS)
    {
      delay(500);
      attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
  }
  void StartAP() override
  {
    Serial.println("Starting AP...");
    //WiFi.disconnect();
   // delay(1000);
    //WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_WIFI_SSID, AP_WIFI_PASSWORD);
  }
};

#endif