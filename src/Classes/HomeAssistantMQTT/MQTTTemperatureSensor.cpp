#include "MQTTTemperatureSensor.h"

void MQTTTemperatureSensor::NotifyObservers(float temp)
{
  for (auto *observer : _observers)
  {
    observer->OnTempChanged(temp);
  }
}

MQTTTemperatureSensor::MQTTTemperatureSensor(HomeAssistantMQTT *mqtt, const std::string &topic) : _mqtt(mqtt), _topic(topic), _temperature(0.0)
{
  _mqtt->Subscribe(_topic.c_str());
  _mqtt->Attach(this);
}

MQTTTemperatureSensor::~MQTTTemperatureSensor()
{
  _mqtt->Detach(this);
}

void MQTTTemperatureSensor::MQTTEvent(const char *topic, const uint8_t *payload, uint16_t length)
{
  if (_topic == std::string(topic))
  {
    float tmp = atof((char *)payload);
    if (tmp != _temperature)
    {
      _temperature = tmp;
      NotifyObservers(_temperature);
    }
  }
}

float MQTTTemperatureSensor::GetTemp()
{
  return _temperature;
}

void MQTTTemperatureSensor::Attach(ITemperatureObserver *observer)
{
  _observers.push_back(observer);
}

void MQTTTemperatureSensor::Detach(ITemperatureObserver *observer)
{
  _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}
