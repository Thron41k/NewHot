#include "BoilerTemperature.h"

BoilerTemperature::BoilerTemperature(Logger logger)
    : _logger(logger), _ds(std::make_unique<GyverDS18Single>(TEMPERATURE_SENSOR))
{
}

void BoilerTemperature::Loop()
{
    if (millis() - _tmr >= _ds->getConversionTime())
    {
        _tmr = millis();
        if (_ds->readTemp())
        {
            float tmp_temp = _ds->getTemp();
            if (_temp != tmp_temp)
            {
                NotifyObservers();
                _temp = tmp_temp;
            }
        }
        else
        {
            _logger.Error("DS18B20 readTemp error");
        }
        if (!_ds->requestTemp())
        {
            _logger.Error("DS18B20 requestTemp error");
        }
    }
}

void BoilerTemperature::NotifyObservers()
{
    for (auto observer : observers)
    {
        observer->Update();
    }
}

void BoilerTemperature::Attach(IObserver *observer)
{
    observers.push_back(observer);
}

void BoilerTemperature::Detach(IObserver *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}
