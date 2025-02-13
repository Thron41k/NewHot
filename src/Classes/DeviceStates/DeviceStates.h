#ifndef DEVICESTATES_H
#define DEVICESTATES_H

#include "ModeType.h"
#include "../BoilerTemperature/BoilerTemperature.h"
#include "../ValveControl/ValveControl.h"
#include <Classes/DeviceConfig/DeviceConfig.h>

class DeviceStates
{
private:
    std::unique_ptr<DeviceConfig> _deviceConfig;
    std::unique_ptr<BoilerTemperature> _boilerTemperature;
    std::unique_ptr<ValveControl> _valveControl;

public:
    DeviceStates()
        : _boilerTemperature(std::make_unique<BoilerTemperature>()), _valveControl(std::make_unique<ValveControl>()), _deviceConfig(std::make_unique<DeviceConfig>()) {}
    ~DeviceStates() = default;
    float GetBoilerTemp() const { return _boilerTemperature->GetTemp(); }
    short GetValvePercent() const { return _valveControl->GetPercent(); }
    void SetValvePercent(short percent) { _valveControl->SetPercentage(percent); }
};

#endif