#include "MQTTTemperatureSensor.h"

MQTTTemperatureSensor::MQTTTemperatureSensor(HomeAssistantMQTT *mqtt, const std::string &topic): _mqtt(mqtt), _topic(topic), _temperature(0.0)
{
    
}

void MQTTTemperatureSensor::NotifyObservers(float temp) {
    for (auto* observer : _observers) {
      observer->OnBoilerTempChanged(temp);
    }
  }
  
  void MQTTTemperatureSensor::Attach(ITemperatureObserver* observer) {
    _observers.push_back(observer);
  }
  
  void MQTTTemperatureSensor::Detach(ITemperatureObserver* observer) {
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
  }

  float MQTTTemperatureSensor::GetTemp()
  {
      return _temperature;
  }
