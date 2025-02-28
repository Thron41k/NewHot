#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

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
    void SetParametre(T value, ParametreType type)
    {
        bool updateNow = false;
        if constexpr (std::is_same<T, const char *>::value)
        {
            switch (type)
            {
            case WiFiPass:
                strcpy(_config.WIFI_PASS, value);
                updateNow = true;
                break;
            case WiFiSSID:
                strcpy(_config.WIFI_SSID, value);
                updateNow = true;
                break;
            }
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            switch (type)
            {
            case MQTT_IP:
                _config.MQTT_IP.fromString(value.c_str());
                updateNow = true;
                break;
            case MQTT_Pass:
                strcpy(_config.MQTT_Pass, value.c_str());
                updateNow = true;
                break;
            case MQTT_User:
                strcpy(_config.MQTT_User, value.c_str());
                updateNow = true;
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
                updateNow = true;
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
        if (updateNow)
            _file_data->updateNow();
        else
            _file_data->update();
    };
    Configuration GetConfig() { return _config; }
    void Loop();
};

#endif
