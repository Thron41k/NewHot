#ifndef DISPLAYRENDERER_H
#define DISPLAYRENDERER_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <memory>
#include "../Helpers/Defines.h"
#include "../Helpers/Utils.h"
#include <Classes/DeviceStates/DeviceStates.h>

class DisplayRenderer
{
private:
    std::unique_ptr<LiquidCrystal_I2C> _lcd; 
public:
    ~DisplayRenderer() = default;
    DisplayRenderer();
    void RenderMode(ModeType mode);
    void RenderModeAutoCurrentTemp(float temp);
    void RenderModeAutoTargetTemp(float temp);
    void RenderModeManualValvePercent(short percent);
    void RenderSlideInfo(std::string info);
};

#endif