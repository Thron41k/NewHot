#include "Classes/DeviceStates/Interfaces/IValveManager.h"
#include <memory>
#include <Classes/ValveControl/ValveControl.h>
class ValveManager : public IValveManager {
private:
  std::unique_ptr<ValveControl> _valve;
public:
  ValveManager() : _valve(std::make_unique<ValveControl>()) {}
  short GetPercent() const override { return _valve->GetPercent(); }
  void SetPercent(short percent) override { _valve->SetPercentage(percent); }
};