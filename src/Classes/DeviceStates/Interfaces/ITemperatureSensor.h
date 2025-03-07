// Classes/DeviceStates/Interfaces/ITemperatureSensor.h
#ifndef ITEMPERATURE_SENSOR_H
#define ITEMPERATURE_SENSOR_H

class ITemperatureSensor
{
public:
  virtual bool RequestTemp() = 0;
  virtual bool ReadTemp() = 0;
  virtual float GetTemp() = 0;
  virtual uint32_t GetConversionTime() = 0;
  virtual ~ITemperatureSensor() = default;
};

#endif