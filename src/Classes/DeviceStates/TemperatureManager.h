// Classes/DeviceStates/TemperatureManager.h
#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include "Classes/DeviceStates/Interfaces/ITemperatureManager.h"
#include "Classes/Helpers/Observers/ITemperatureObserver.h"
#include "Classes/BoilerTemperature/BoilerTemperature.h"
#include "DS18B20Sensor.h"
#include <Classes/HomeTemperature/HomeTemperature.h>
#include <Classes/HomeAssistantMQTT/MQTTTemperatureSensor.h>

class TemperatureManager : public ITemperatureManager, public ITemperatureObserver
{
private:
    Logger _logger;
    std::unique_ptr<BoilerTemperature> _boilerTemp;
    std::unique_ptr<HomeTemperature> _homeTempSensor;
    float _homeTemp = 0.0;
    float _outdoorTemp = 0.0;
    std::vector<ITemperatureObserver *> _observers;

public:
    TemperatureManager(Logger logger, uint8_t pin = TEMPERATURE_SENSOR) // Добавляем pin с значением по умолчанию
        : _logger(logger),
          _boilerTemp(std::make_unique<BoilerTemperature>(logger, std::make_unique<DS18B20Sensor>(pin)))
    {
        _boilerTemp->Attach(this);
    }

    float GetBoilerTemp() const override { return _boilerTemp->GetTemp(); }
    float GetHomeTemp() const override { return _homeTemp; }
    float GetOutdoorTemp() const override { return _outdoorTemp; }
    void Loop() override { _boilerTemp->Loop(); }
    void Attach(ITemperatureObserver *observer) override { _observers.push_back(observer); }
    void Detach(ITemperatureObserver *observer) override
    {
        _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
    }

    void SetHomeTemp(float temp) { _homeTemp = temp; }
    void SetOutdoorTemp(float temp) { _outdoorTemp = temp; }

    void OnTempChanged(float temp) override
    {
        Serial.println("Temperature changed: " + String(temp));
        for (auto *obs : _observers)
        {
            obs->OnTempChanged(temp);
        }
    }
    void SetHomeTemperatureSensor(std::unique_ptr<HomeTemperature> sensor) override
    {
        if (sensor)
        {
            _homeTempSensor = std::move(sensor);
            _homeTempSensor->Attach(this);
        }
    }
};
#endif