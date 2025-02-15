#include <EEPROM.h>
#include "Configuration.h"
#include "ParametreType.h"

class DeviceConfig
{
private : Configuration _config;
    int const _device_uid = 77234566;
    void LoadConfig();
    void SaveConfig();
    uint32_t _last_save;
    bool _need_save = false;
public:
    DeviceConfig();
    ~DeviceConfig() = default;
    template <typename T>
    void SetParametre(T value, ParametreType type);
    void Loop();
};
