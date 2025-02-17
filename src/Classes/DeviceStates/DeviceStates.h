#ifndef DEVICESTATES_H
#define DEVICESTATES_H

#include "../BoilerTemperature/BoilerTemperature.h"
#include "../ValveControl/ValveControl.h"
#include <Classes/DeviceConfig/DeviceConfig.h>
#include <Classes/Helpers/Observer/IObserver.h>
#include <Classes/Helpers/Observer/IEncoderEventObserver.h>

class DeviceStates : public IObserver, public IEncoderEventObserver
{
private:
    Logger _logger;
    std::unique_ptr<DeviceConfig> _deviceConfig;
    std::unique_ptr<BoilerTemperature> _boilerTemperature;
    std::unique_ptr<ValveControl> _valveControl;
    float _currentHomeTemp = 0.0;
    float _outdoorTemp = 0.0;
    std::vector<IObserver *> observers;
    void NotifyObservers();

public:
    DeviceStates(Logger logger)
        : _boilerTemperature(std::make_unique<BoilerTemperature>(logger)), _valveControl(std::make_unique<ValveControl>()), _deviceConfig(std::make_unique<DeviceConfig>())
    {
        const_cast<BoilerTemperature &>(*_boilerTemperature).Attach(this);
    }
    ~DeviceStates() = default;
    float GetBoilerTemp() const { return _boilerTemperature->GetTemp(); }
    short GetValvePercent() const { return _valveControl->GetPercent(); }
    void SetValvePercent(short percent);
    void SetMode(ModeType mode);
    void SetHomeTargetTemp(float temp);
    void SetBoilerMaxTemp(float temp);
    void SetCurrentHomeTemp(float temp) { _currentHomeTemp = temp; }
    float GetCurrentHomeTemp() const { return _currentHomeTemp; }
    DeviceConfig *GetDeviceConfig() const { return _deviceConfig.get(); }
    float GetOutdoorTemp() const { return _outdoorTemp; }
    void Loop();
    void Update() override;
    void Attach(IObserver *observer);
    void Detach(IObserver *observer);
};

#endif