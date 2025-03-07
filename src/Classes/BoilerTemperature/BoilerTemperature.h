// Classes/BoilerTemperature/BoilerTemperature.h
#ifndef BOILER_TEMPERATURE_H
#define BOILER_TEMPERATURE_H

#include <memory>
#include "../DeviceStates/Interfaces/ITemperatureSensor.h"
#include "../Helpers/Logger.h"
#include "../Helpers/Observers/ITemperatureObserver.h" // Новый интерфейс

class BoilerTemperature
{
private:
    Logger _logger;
    uint32_t _tmr;
    float _temp;
    std::unique_ptr<ITemperatureSensor> _sensor;
    std::vector<ITemperatureObserver *> _observers;
    void NotifyObservers(float temp);

public:
    BoilerTemperature(Logger logger, std::unique_ptr<ITemperatureSensor> sensor);
    void Loop();
    float GetTemp() const { return _temp; }
    void Attach(ITemperatureObserver *observer);
    void Detach(ITemperatureObserver *observer);
};

#endif