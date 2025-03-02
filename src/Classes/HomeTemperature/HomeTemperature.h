#ifndef HOMETEMPERATURE_H
#define HOMETEMPERATURE_H

#include <memory>
#include "../Helpers/Logger.h"
#include "Classes/DeviceStates/Interfaces/IMQTTTemperatureSensor.h" // Новый интерфейс
#include <Classes/Helpers/Observers/ITemperatureObserver.h>

class HomeTemperature : public ITemperatureObserver
{
private:
    Logger _logger;
    uint32_t _tmr;
    float _temp;
    std::unique_ptr<IMQTTTemperatureSensor> _sensor;
    std::vector<ITemperatureObserver *> _observers;
    void NotifyObservers(float temp);

public:
    HomeTemperature(Logger logger, std::unique_ptr<IMQTTTemperatureSensor> sensor);
    ~HomeTemperature();
    void Loop();
    float GetTemp() const { return _temp; }
    void Attach(ITemperatureObserver *observer);
    void Detach(ITemperatureObserver *observer);
    void OnTempChanged(float temp) override;
};

#endif