#ifndef MQTT_TEMPERATURE_SENSOR_H
#define MQTT_TEMPERATURE_SENSOR_H

#include <string>
#include "../HomeAssistantMQTT/HomeAssistantMQTT.h"
#include "../DeviceStates/Interfaces/IMQTTTemperatureSensor.h"
#include <Classes/Helpers/Observers/ITemperatureObserver.h>

class MQTTTemperatureSensor : public IMQTTTemperatureSensor, public IMQTTObserver
{
private:
    HomeAssistantMQTT *_mqtt; // Указатель на HomeAssistantMQTT
    std::string _topic;       // Топик для подписки (например, "home/temperature/indoor")
    float _temperature;       // Текущее значение температуры
    std::vector<ITemperatureObserver *> _observers;
    void NotifyObservers(float temp);
public:
    MQTTTemperatureSensor(HomeAssistantMQTT *mqtt, const std::string &topic);
    ~MQTTTemperatureSensor();
    void MQTTEvent(const char* topic, const uint8_t* payload, uint16_t length) override;
    float GetTemp() override;
    void Attach(ITemperatureObserver *observer);
    void Detach(ITemperatureObserver *observer);
};

#endif