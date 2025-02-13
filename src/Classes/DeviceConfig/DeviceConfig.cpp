#include "DeviceConfig.h"

void DeviceConfig::LoadConfig()
{
    EEPROM.begin(4096);
    EEPROM.get(0, _config);
    EEPROM.end();
    if (_config.device_uid != _device_uid)
    {
        SaveConfig(true);
    }
}

void DeviceConfig::SaveConfig(bool newConfig = false)
{
    if (newConfig)
    {
        _config = Configuration();
    }
    EEPROM.begin(4096);
    EEPROM.put(0, _config);
    EEPROM.end();
}

template <typename T, ParametreType>
void DeviceConfig::SetParametre(T value, ParametreType type)
{
    switch (type)
    {
    case ModeType:
        _config.mode = static_cast<ModeType>(value);
        break;
    case BoilerMaxTemp:
        _config.boilerMaxTemp = static_cast<float>(value);
        break;
    case HomeTargetTemp:
        _config.homeTargetTemp = static_cast<float>(value);
        break;
    case ValvePercent:
        _config.valvePercent = static_cast<short>(value);
        break;
    default:
        break;
    }
}
DeviceConfig::DeviceConfig()
{
    LoadConfig();
}