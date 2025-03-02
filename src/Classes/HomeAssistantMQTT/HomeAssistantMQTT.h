#ifndef HOMEASSISTANTMQTT_H
#define HOMEASSISTANTMQTT_H

#include <WiFiClient.h>
#include <HADevice.h>
#include <HAMqtt.h>
#include <memory>
#include <vector>
#include "Interfaces/IMQTTDevice.h"
#include "Classes/Helpers/Observers/IMQTTObserver.h"

class HomeAssistantMQTT
{
private:
    WiFiClient _client;
    std::unique_ptr<HADevice> _hadevice;
    std::unique_ptr<HAMqtt> _hamqtt;
    std::vector<std::unique_ptr<IMQTTDevice>> _devices;
    std::vector<IMQTTObserver *> _observers;

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
        _hamqtt->onMessage(onMqttMessageWrapper, this);
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
        for (auto &device : _devices)
        {
            device->Report();
        }
    }

    void loop()
    {
        for (auto &device : _devices)
        {
            device->loop();
        }
        _hamqtt->loop();
    }
    void onMqttMessage(const char *topic, const uint8_t *payload, uint16_t length)
    {
        NotifyObservers(topic, payload, length);
    }

    static void onMqttMessageWrapper(void *context, const char *topic, const uint8_t *payload, uint16_t length)
    {
        // Приводим контекст к типу HomeAssistantMQTT и вызываем метод
        static_cast<HomeAssistantMQTT *>(context)->onMqttMessage(topic, payload, length);
    }

    void Subscribe(const char *topic)
    {
        _hamqtt->subscribe(topic);
    }

    void Attach(IMQTTObserver *observer) { _observers.push_back(observer); };
    void Detach(IMQTTObserver *observer) { _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end()); };
    void NotifyObservers(const char *topic, const uint8_t *payload, uint16_t length)
    {
        for (auto observer : _observers)
        {
            observer->MQTTEvent(topic, payload, length);
        }
    };
};

#endif