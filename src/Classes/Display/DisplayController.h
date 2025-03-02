#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include "Classes/Helpers/Observers/ITemperatureObserver.h"
#include "Classes/Helpers/Observers/IValveObserver.h"
#include "Classes/Helpers/Observers/IConfigObserver.h"
#include "Classes/Helpers/Observers/IEncoderObserver.h"
#include <Classes/DeviceStates/Interfaces/ITemperatureManager.h>
#include <Classes/DeviceStates/Interfaces/IValveManager.h>
#include <Classes/DeviceStates/Interfaces/IConfigManager.h>
#include <Classes/Input/EncoderControl.h>
#include "DisplayStateEnum.h"
#include "Interfaces/IDisplayRenderer.h"


class DisplayController : public ITemperatureObserver, 
                         public IValveObserver, 
                         public IConfigObserver {
private:
  std::unique_ptr<IDisplayRenderer> _renderer;
  const ITemperatureManager& _tempMgr;
  const IValveManager& _valveMgr;
  const IConfigManager& _configMgr;
  DisplayStateEnum _displayState = MainScreen;
  short _slidePos = 0;
  uint32_t _lastSlideUpdate = 0;

  void UpdateSlideShow();

public:
  DisplayController(std::unique_ptr<IDisplayRenderer> renderer,
                    const ITemperatureManager& tempMgr,
                    const IValveManager& valveMgr,
                    const IConfigManager& configMgr)
    : _renderer(std::move(renderer)), _tempMgr(tempMgr), _valveMgr(valveMgr),
      _configMgr(configMgr) {
    _renderer->RenderMode(_configMgr.GetMode());
  }

  void OnTempChanged(float temp) override {
    if (_displayState == MainScreen) {
      _renderer->RenderBoilerTemp(temp);
    }
  }

  void OnValvePercentChanged(short percent) override {
    if (_displayState == MainScreen && _configMgr.GetMode() == ModeType::Manual) {
      _renderer->RenderValvePercent(percent);
    }
  }

  void OnConfigChanged(ParametreType param) override {
    if (_displayState == MainScreen) {
      switch (param) {
        case Mode:
          _renderer->RenderMode(_configMgr.GetMode());
          if (_configMgr.GetMode() == ModeType::Auto) {
            _renderer->RenderHomeTemp(_tempMgr.GetHomeTemp());
            _renderer->RenderTargetHomeTemp(_configMgr.GetTargetHomeTemp());
          } else {
            _renderer->RenderValvePercent(_valveMgr.GetPercent());
          }
          break;
        case HomeTargetTemp:
          if (_configMgr.GetMode() == ModeType::Auto) {
            _renderer->RenderTargetHomeTemp(_configMgr.GetTargetHomeTemp());
          }
          break;
        case BoilerMaxTemp:
          break;
      }
    }
  }

  void Loop() {
    if (_displayState == MainScreen) {
      UpdateSlideShow();
    }
  }
};
#endif