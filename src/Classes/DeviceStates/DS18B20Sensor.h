//Classes/DeviceStates/DS18B20Sensor.h
#ifndef DS18B20_SENSOR_H
#define DS18B20_SENSOR_H

#include "interfaces/ITemperatureSensor.h"
#include <GyverDS18.h>
#include "../Helpers/Defines.h"

class DS18B20Sensor : public ITemperatureSensor
{
private:
    GyverDS18Single _ds;

public:
    DS18B20Sensor(uint8_t pin) : _ds(pin) {}
    bool RequestTemp() override { return _ds.requestTemp(); }
    bool ReadTemp() override { return _ds.readTemp(); }
    float GetTemp() override { return _ds.getTemp(); }
    uint32_t GetConversionTime() override { return _ds.getConversionTime(); }
};

#endif