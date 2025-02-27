#ifndef STATUSPROVIDER_H
#define STATUSPROVIDER_H

#include "Interfaces/IWiFiManager.h"
#include "Interfaces/IStatusProvider.h"
#include <Arduino.h>
class StatusProvider : public IStatusProvider
{
private:
  const IWiFiManager &_wifiManager;
  mutable char _jsonBuffer[512];

public:
  StatusProvider(const IWiFiManager &wifiManager) : _wifiManager(wifiManager) {}
  const char *GetStatusJson() const override;
};
#endif