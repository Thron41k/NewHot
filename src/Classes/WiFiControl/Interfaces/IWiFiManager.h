// IWiFiManager.h
#ifndef IWIFIMANAGER_H
#define IWIFIMANAGER_H

#include <IPAddress.h>
#include <string>
#include <vector>
#include <Classes/WiFiControl/NetworkInfo.h>

class IWiFiManager
{
public:
    virtual bool ConnectToAP(const std::string &ssid, const std::string &password) = 0; // Оставляем неконстантным
    virtual std::vector<NetworkInfo> ScanNetworks() = 0;
    virtual bool IsConnected() const = 0;
    virtual String GetSSID() const = 0;
    virtual IPAddress GetIP() const = 0;
    virtual void Loop() = 0;
    virtual bool isWifiReady() const = 0;
    virtual ~IWiFiManager() = default;
};

#endif