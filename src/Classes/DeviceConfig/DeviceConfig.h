#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

#include "Configuration.h"
#include "ParametreType.h"
#include "FileData.h"
#include <SPIFFS.h>
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
    void SetParametre(T value, ParametreType type)
    {
        if constexpr (std::is_same<T, std::string>::value)
        {
            switch (type)
            {
            case WiFiPass:
                strcpy(_config.WIFI_PASS, value.c_str());
                break;
            case WiFiSSID:
                strcpy(_config.WIFI_SSID, value.c_str());
                break;
            case MQTT_IP:
                _config.MQTT_IP.fromString(value.c_str());
                break;
            case MQTT_Pass:
                strcpy(_config.MQTT_Pass, value.c_str());
                break;
            case MQTT_User:
                strcpy(_config.MQTT_User, value.c_str());
                break;
            }
        }
        else if constexpr (std::is_same<T, float>::value)
        {
            switch (type)
            {
            case BoilerMaxTemp:
                _config.boilerMaxTemp = value;
                break;
            case HomeTargetTemp:
                _config.homeTargetTemp = value;
                break;
            }
        }
        else if constexpr (std::is_same<T, int>::value)
        {
            switch (type)
            {
            case ValvePercent:
                _config.valvePercent = value;
                break;
            case MQTT_Port:
                _config.MQTT_Port = value;
                break;
            }
        }
        else if constexpr (std::is_same<T, ModeType>::value)
        {
            if (type == Mode)
            {
                _config.mode = static_cast<ModeType>(value);
            }
        }
        _file_data->update();
    };
    Configuration GetConfig() { return _config; }
    void Loop();
};

#endif
