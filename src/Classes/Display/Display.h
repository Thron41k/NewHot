#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <memory>
#include "../Helpers/Defines.h"
#include "../Helpers/Utils.h"
#include <Classes/DeviceStates/DeviceStates.h>
#include <Classes/Helpers/Observer/IObserver.h>

class Display : public IObserver 
{
private:
    std::unique_ptr<LiquidCrystal_I2C> _lcd;
    const DeviceStates *_deviceStates; 
    void PrintHomeTemp();
    void PrintValvePercent();
    uint32_t _rotation_timeout;
    bool _rotation_state = false;
public:
    ~Display();
    Display(const DeviceStates &_deviceStates);
    void Update() override;
    void Loop();
};

#endif