#ifndef DISPLAYRENDERER_H
#define DISPLAYRENDERER_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <memory>
#include "../Helpers/Defines.h"
#include "../Helpers/Utils.h"
#include <Classes/DeviceStates/DeviceStates.h>
#include <Classes/Helpers/Observer/IObserver.h>

class DisplayRenderer
{
private:
    std::unique_ptr<LiquidCrystal_I2C> _lcd;
    const DeviceStates *_deviceStates; 
    void PrintHomeTemp();
    void PrintValvePercent();
    uint32_t _rotation_timeout;
    bool _rotation_state = false;
public:
    ~DisplayRenderer() = default;
    DisplayRenderer(const DeviceStates &_deviceStates);
    void Loop();
};

#endif