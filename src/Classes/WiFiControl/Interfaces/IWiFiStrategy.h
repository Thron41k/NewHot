//Classes/WiFiControl/Interfaces/IWiFiStrategy.h
#ifndef IWIFISTRATEGY_H
#define IWIFISTRATEGY_H

#include <string>

class IWiFiStrategy {
public:
  virtual bool Connect(const std::string& ssid, const std::string& password) = 0;
  virtual void StartAP() = 0;
  virtual ~IWiFiStrategy() = default;
};

#endif