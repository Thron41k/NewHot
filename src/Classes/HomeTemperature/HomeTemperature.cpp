#include "HomeTemperature.h"

void HomeTemperature::NotifyObservers(float temp)
{
    for (auto *observer : _observers)
    {
        observer->OnTempChanged(temp);
    }
}

HomeTemperature::HomeTemperature(Logger logger, std::unique_ptr<IMQTTTemperatureSensor> sensor)
    : _logger(logger), _sensor(std::move(sensor)), _temp(0.0), _tmr(0) {
        _sensor->Attach(this);
    }

    HomeTemperature::~HomeTemperature()
    {
        _sensor->Detach(this);
    }

void HomeTemperature::Loop()
{
}

void HomeTemperature::Attach(ITemperatureObserver *observer)
{
    _observers.push_back(observer);
}

void HomeTemperature::Detach(ITemperatureObserver *observer)
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

void HomeTemperature::OnTempChanged(float temp)
{
    NotifyObservers(temp);
}
