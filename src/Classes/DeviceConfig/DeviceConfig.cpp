#include "DeviceConfig.h"
#include <Classes/Helpers/Defines.h>

void DeviceConfig::LoadConfig()
{
    EEPROM.begin(4096);
    EEPROM.get(0, _config);
    EEPROM.end();
    if (_config.device_uid != _device_uid)
    {
        _config = Configuration();
        SaveConfig();
    }
}

void DeviceConfig::SaveConfig()
{
    EEPROM.begin(4096);
    EEPROM.put(0, _config);
    EEPROM.end();
}

template <typename T>
void DeviceConfig::SetParametre(T value, ParametreType type)
{
    switch (type)
    {
    case Mode:
        _config.mode = value;
        break;
    case BoilerMaxTemp:
        _config.boilerMaxTemp = value;
        break;
    case HomeTargetTemp:
        _config.homeTargetTemp = value;
        break;
    case ValvePercent:
        _config.valvePercent = value;
        break;
    default:
        break;
    }
    _need_save = true;
    _last_save = millis();
}
DeviceConfig::DeviceConfig()
{
    LoadConfig();
}

void DeviceConfig::Loop()
{
    if (_need_save && millis() - _last_save >= CONFIG_SAVE_TIMEOUT)
    {
        _need_save = false;
        _last_save = millis();
        SaveConfig();
    }
}
