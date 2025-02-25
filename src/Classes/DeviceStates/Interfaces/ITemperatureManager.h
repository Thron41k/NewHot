#ifndef ITEMPERATURE_MANAGER_H
#define ITEMPERATURE_MANAGER_H

#include "Classes/Helpers/Observers/ITemperatureObserver.h"

class ITemperatureManager {
public:
  virtual float GetBoilerTemp() const = 0;
  virtual float GetHomeTemp() const = 0;
  virtual float GetOutdoorTemp() const = 0;
  virtual void Loop() = 0;
  virtual void Attach(ITemperatureObserver* observer) = 0; // Добавляем
  virtual void Detach(ITemperatureObserver* observer) = 0; // Добавляем
  virtual ~ITemperatureManager() = default;
};

#endif