#ifndef MQTT_TEMPERATURE_SENSOR_H
#define MQTT_TEMPERATURE_SENSOR_H

#include <string>
#include "../HomeAssistantMQTT/HomeAssistantMQTT.h"
#include "../DeviceStates/Interfaces/ITemperatureSensor.h"
#include <Classes/Helpers/Observers/ITemperatureObserver.h>

class MQTTTemperatureSensor : public ITemperatureSensor
{
private:
    HomeAssistantMQTT *_mqtt; // Указатель на HomeAssistantMQTT
    std::string _topic;       // Топик для подписки (например, "home/temperature/indoor")
    float _temperature;       // Текущее значение температуры
    std::unique_ptr<ITemperatureSensor> _sensor;
    std::vector<ITemperatureObserver *> _observers;

public:
    MQTTTemperatureSensor(HomeAssistantMQTT *mqtt, const std::string &topic);
    void NotifyObservers(float temp);
    void Attach(ITemperatureObserver *observer);
    void Detach(ITemperatureObserver *observer);
    float GetTemp() override;
};

#endif