#include "BoilerTemperature.h"
#include <Classes/Helpers/Defines.h>

BoilerTemperature::BoilerTemperature(Logger logger, std::unique_ptr<ITemperatureSensor> sensor)
    : _logger(logger), _sensor(std::move(sensor)), _temp(0.0), _tmr(0) {}

void BoilerTemperature::Loop() {
  if (millis() - _tmr >= _sensor->GetConversionTime() && !TEST_MODE) {
    _tmr = millis();
    if (_sensor->ReadTemp()) {
      float tmp_temp = _sensor->GetTemp();
      if (_temp != tmp_temp) {
        _temp = tmp_temp;
        NotifyObservers(_temp);
      }
    } else {
      _logger.Error("Temperature sensor read error");
    }
    if (!_sensor->RequestTemp()) {
      _logger.Error("Temperature sensor request error");
    }
  }
}

void BoilerTemperature::NotifyObservers(float temp) {
  for (auto* observer : _observers) {
    observer->OnTempChanged(temp);
  }
}

void BoilerTemperature::Attach(ITemperatureObserver* observer) {
  _observers.push_back(observer);
}

void BoilerTemperature::Detach(ITemperatureObserver* observer) {
  _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}