#ifndef DS18B20_SENSOR_H
#define DS18B20_SENSOR_H

#include "interfaces/ITemperatureSensor.h"
#include <GyverDS18.h>
#include "../Helpers/Defines.h"

class DS18B20Sensor : public ITemperatureSensor {
    private:
      // Внутренний класс-обёртка для GyverDS18Single
      class DS18Wrapper {
      public:
        GyverDS18Single _ds;
        DS18Wrapper(uint8_t pin) : _ds(pin) {} // Передаём номер пина в конструктор
        bool requestTemp() { return _ds.requestTemp(); }
        bool readTemp() { return _ds.readTemp(); }
        float getTemp() const { return _ds.getTemp(); }
        uint32_t getConversionTime() const { return _ds.getConversionTime(); }
      };
    
      DS18Wrapper _wrapper; // Экземпляр обёртки
    
    public:
      DS18B20Sensor(uint8_t pin) : _wrapper(pin) {} // Передаём пин в конструктор
      bool RequestTemp() override { return _wrapper.requestTemp(); }
      bool ReadTemp() override { return _wrapper.readTemp(); }
      float GetTemp() const override { return _wrapper.getTemp(); }
      uint32_t GetConversionTime() const override { return _wrapper.getConversionTime(); }
    };
    
    #endif