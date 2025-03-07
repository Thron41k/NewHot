// Classes/DeviceStates/Interfaces/IMQTTTemperatureSensor.h
#ifndef IMQTT_TEMPERATURE_SENSOR_H
#define IMQTT_TEMPERATURE_SENSOR_H

#include <Classes/Helpers/Observers/IMQTTObserver.h>
#include "Classes/Helpers/Observers/ITemperatureObserver.h"

class IMQTTTemperatureSensor
{
public:
  virtual float GetTemp() = 0;
  virtual ~IMQTTTemperatureSensor() = default;
  virtual void Attach(ITemperatureObserver *observer) = 0;
  virtual void Detach(ITemperatureObserver *observer) = 0;
};

#endif