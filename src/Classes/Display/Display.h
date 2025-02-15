#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <memory>
#include <algorithm>
#include "../Helpers/Defines.h"
#include <Classes/DeviceStates/ModeType.h>
#include <Classes/DeviceStates/DeviceStates.h>

class Display
{
private:
    std::unique_ptr<LiquidCrystal_I2C> _lcd;
    
public:
    Display(const DeviceStates& _deviceStates);
    ~Display() = default;
    void UpdateMode(ModeType mode);
};