#ifndef VALVEMANAGER_H
#define VALVEMANAGER_H

#include "Classes/DeviceStates/Interfaces/IValveManager.h"
#include <memory>
#include <Classes/ValveControl/ValveControl.h>
class ValveManager : public IValveManager
{
private:
  std::unique_ptr<ValveControl> _valve;

public:
  ValveManager();
  short GetPercent() const override { return _valve->GetPercent(); }
  void SetPercent(short percent) override { _valve->SetPercentage(percent); }
};

#endif