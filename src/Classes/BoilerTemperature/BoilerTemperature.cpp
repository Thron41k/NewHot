#include "BoilerTemperature.h"

BoilerTemperature::BoilerTemperature(Logger logger)
    : _logger(logger), _ds(std::make_unique<GyverDS18Single>(4))
{
}

void BoilerTemperature::Loop()
{
    if (millis() - _tmr >= _ds->getConversionTime())
    {
        _tmr = millis();
        if (_ds->readTemp())
        {
            _temp = _ds->getTemp();
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
