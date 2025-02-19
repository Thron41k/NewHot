#include <EEPROM.h>
#include "Configuration.h"
#include "ParametreType.h"
#include "FileData.h"
#include <LittleFS.h>
#include <Classes/Helpers/Defines.h>

class DeviceConfig
{
private:
    Configuration _config;
    std::unique_ptr<FileData> _file_data;
public:
    DeviceConfig();
    ~DeviceConfig() = default;
    template <typename T>
    void SetParametre(T value, ParametreType type);
    Configuration GetConfig() { return _config; }
    void Loop();
};
