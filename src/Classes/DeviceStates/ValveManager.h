// Classes/DeviceStates/ValveManager.h
#ifndef VALVEMANAGER_H
#define VALVEMANAGER_H

#include "Classes/DeviceStates/Interfaces/IValveManager.h"
#include <memory>
#include <Classes/ValveControl/ValveControl.h>
#include "Interfaces/IConfigManager.h"
class ValveManager : public IValveManager
{
private:
  std::unique_ptr<ValveControl> _valve;
  IConfigManager *_configMgr;

public:
  ValveManager(IConfigManager *configMgr = nullptr);
  short GetPercent() const override { return _configMgr->GetValvePositionPercent(); }
  void SetPercent(short percent) override;
};

#endif