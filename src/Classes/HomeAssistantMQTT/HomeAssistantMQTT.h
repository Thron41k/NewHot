#ifndef HOMEASSISTANTMQTT_H
#define HOMEASSISTANTMQTT_H

#include <WiFiClient.h>
#include <HADevice.h>
#include <HAMqtt.h>
#include <memory>
#include <vector>
#include "Interfaces/IMQTTDevice.h"

class HomeAssistantMQTT
{
private:
    WiFiClient _client;
    std::unique_ptr<HADevice> _hadevice;
    std::unique_ptr<HAMqtt> _hamqtt;
    std::vector<std::unique_ptr<IMQTTDevice>> _devices;

    // Преобразует строку в массив байтов
    std::vector<byte> stringToByteArray(const std::string &str)
    {
        return std::vector<byte>(str.begin(), str.end());
    }

public:
    HomeAssistantMQTT(const std::string &deviceName, const std::string &uniqueId)
    {
        _hadevice = std::make_unique<HADevice>();
        _hamqtt = std::make_unique<HAMqtt>(_client, *_hadevice);
        _hadevice->setName(deviceName.c_str());

        // Преобразуем uniqueId в массив байтов
        auto uniqueIdBytes = stringToByteArray(uniqueId);
        _hadevice->setUniqueId(uniqueIdBytes.data(), uniqueIdBytes.size());
    }

    void addDevice(std::unique_ptr<IMQTTDevice> device)
    {
        device->setup(*_hadevice, *_hamqtt);
        _devices.push_back(std::move(device));
    }

    void begin(const std::string &mqttIP, const std::string &mqttUser, const std::string &mqttPass)
    {
        IPAddress mqttIpl;
        if (!mqttIpl.fromString(mqttIP.c_str()))
        {
            mqttIpl = IPAddress(0, 0, 0, 0);
        }
        _hamqtt->begin(mqttIpl, mqttUser.c_str(), mqttPass.c_str());
    }

    void loop()
    {
        for (auto &device : _devices)
        {
            device->loop();
        }
        _hamqtt->loop();
    }
};

#endif