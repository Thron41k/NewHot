// Classes/DeviceConfig/DeviceConfig.cpp
#include "DeviceConfig.h"

DeviceConfig::DeviceConfig()
{
    _file_data = std::make_unique<FileData>(&LittleFS, "/config.bin", 'B', &_config, sizeof(_config));
    _file_data->addWithoutWipe(true);
    _file_data->read();
    _file_data->setTimeout(CONFIG_SAVE_TIMEOUT);
}

void DeviceConfig::Loop()
{
    _file_data->tick();
}
