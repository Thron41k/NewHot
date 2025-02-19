#include "DeviceConfig.h"

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
    case WiFiPass:
        strcpy(_config.WIFI_PASS, value);
        break;
    case WiFiSSID:
        strcpy(_config.WIFI_SSID, value);
        break;
    case MQTT_IP:
        _config.MQTT_IP.fromString(value.c_str());
        break;
    case MQTT_Port:
        _config.MQTT_Port = value;
        break;
    case MQTT_Pass:
        strcpy(_config.MQTT_Pass, value);
        break;
    case MQTT_User:
        strcpy(_config.MQTT_User, value);
        break;
    default:
        break;
    }
    _file_data->update();
}
DeviceConfig::DeviceConfig()
{
    LittleFS.begin();
    _file_data = std::make_unique<FileData>(&LittleFS, "/config.bin", 'B', &_config, sizeof(_config));
    _file_data->addWithoutWipe(true);
    _file_data->read();
    _file_data->setTimeout(CONFIG_SAVE_TIMEOUT);
}

void DeviceConfig::Loop()
{
    _file_data->tick();
}
