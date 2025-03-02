#ifndef ITEMPERATURE_MANAGER_H
#define ITEMPERATURE_MANAGER_H

#include "Classes/Helpers/Observers/ITemperatureObserver.h"
#include "Classes/HomeTemperature/HomeTemperature.h"

class ITemperatureManager {
public:
  virtual float GetBoilerTemp() const = 0;
  virtual float GetHomeTemp() const = 0;
  virtual float GetOutdoorTemp() const = 0;
  virtual void Loop() = 0;
  virtual void Attach(ITemperatureObserver* observer) = 0; // Добавляем
  virtual void Detach(ITemperatureObserver* observer) = 0; // Добавляем
  virtual ~ITemperatureManager() = default;
  virtual void SetHomeTemperatureSensor(std::unique_ptr<HomeTemperature> sensor) = 0;
};

#endif