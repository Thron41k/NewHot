#include <EEPROM.h>
#include "Configuration.h"
#include "ParametreType.h"

class DeviceConfig
{
private : Configuration _config;
    int const _device_uid = 77234566436;
    void LoadConfig();
    void SaveConfig(bool newConfig = false);

public:
    DeviceConfig();
    ~DeviceConfig() = default;
    template <typename T, ParametreType>
    void SetParametre(T value, ParametreType type);
};
