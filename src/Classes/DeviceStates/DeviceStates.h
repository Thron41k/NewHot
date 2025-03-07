// Classes/DeviceStates/DeviceStates.h
#ifndef DEVICESTATES_H
#define DEVICESTATES_H

#include <vector>
#include "Interfaces/ITemperatureManager.h"
#include "Interfaces/IValveManager.h"
#include "Interfaces/IConfigManager.h"
#include <Classes/Helpers/Observers/ITemperatureObserver.h>
#include <Classes/Helpers/Observers/IValveObserver.h>
#include <Classes/Helpers/Observers/IConfigObserver.h>
#include "Classes/Input/EncoderControl.h"

class DeviceStates : public IEncoderObserver
{
private:
  std::unique_ptr<ITemperatureManager> _tempManager;
  std::unique_ptr<IValveManager> _valveManager;
  std::unique_ptr<IConfigManager> _configManager;
  std::vector<ITemperatureObserver *> _tempObservers;
  std::vector<IValveObserver *> _valveObservers;
  std::vector<IConfigObserver *> _configObservers;
  const EncoderControl &_enc;

  void NotifyTempObservers(float temp)
  {
    for (auto *obs : _tempObservers)
      obs->OnTempChanged(temp);
  }
  void NotifyValveObservers(short percent)
  {
    for (auto *obs : _valveObservers)
      obs->OnValvePercentChanged(percent);
  }
  void NotifyConfigObservers(ParametreType param)
  {
    for (auto *obs : _configObservers)
      obs->OnConfigChanged(param);
  }

public:
  DeviceStates(std::unique_ptr<ITemperatureManager> tempMgr,
               std::unique_ptr<IValveManager> valveMgr,
               std::unique_ptr<IConfigManager> configMgr,
               const EncoderControl &enc)
      : _tempManager(std::move(tempMgr)), _valveManager(std::move(valveMgr)),
        _configManager(std::move(configMgr)), _enc(enc)
  {
    const_cast<EncoderControl &>(_enc).Attach(this);
  }

  ~DeviceStates()
  {
    const_cast<EncoderControl &>(_enc).Detach(this);
  }

  void Loop()
  {
    _tempManager->Loop();
    _configManager->Loop();
  }

  void OnEncoderEvent(EncoderEventEnum event) override
  {
    ModeType mode = _configManager->GetMode();
    if (event == EncoderEventEnum::Left)
    {
      if (mode == ModeType::Auto)
      {
        _configManager->SetTargetHomeTemp(_configManager->GetTargetHomeTemp() - 1);
        NotifyConfigObservers(HomeTargetTemp);
      }
      else if (mode == ModeType::Manual)
      {
        short newPercent = max(0, _valveManager->GetPercent() - 1);
        _valveManager->SetPercent(newPercent);
        NotifyValveObservers(newPercent);
      }
    }
    else if (event == EncoderEventEnum::Right)
    {
      if (mode == ModeType::Auto)
      {
        _configManager->SetTargetHomeTemp(_configManager->GetTargetHomeTemp() + 1);
        NotifyConfigObservers(HomeTargetTemp);
      }
      else if (mode == ModeType::Manual)
      {
        short newPercent = min(100, _valveManager->GetPercent() + 1);
        _valveManager->SetPercent(newPercent);
        NotifyValveObservers(newPercent);
      }
    }
  }

  void Attach(ITemperatureObserver *obs)
  {
    _tempObservers.push_back(obs);
    _tempManager->Attach(obs); // Передаём наблюдателя в TemperatureManager
  }
  void Attach(IValveObserver *obs) { _valveObservers.push_back(obs); }
  void Attach(IConfigObserver *obs) { _configObservers.push_back(obs); }
  ITemperatureManager *getTempMgr() const { return _tempManager.get(); }
  IValveManager *getValveMgr() const { return _valveManager.get(); }
  IConfigManager *getConfigMgr() const
  {
    return _configManager.get();
  }
};

#endif