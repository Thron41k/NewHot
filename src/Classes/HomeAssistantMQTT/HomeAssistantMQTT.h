#include <WiFi.h>
#include <HADevice.h>
#include <HAMqtt.h>
#include <memory>
#include "Classes/DeviceStates/DeviceStates.h"
#include <device-types/HANumber.h>

class HomeAssistantMQTT : public IObserver
{
public:
    HomeAssistantMQTT(DeviceStates &deviceStates);
    void onNumberCommand(HANumeric number, HANumber *sender);
    ~HomeAssistantMQTT() = default;
    void Update(ParametreType param_type) override;
private:
    DeviceStates *_deviceStates;
    WiFiClient client;
    std::unique_ptr<HANumber> _airValve;
    std::unique_ptr<HADevice> _hadevice;
    std::unique_ptr<HAMqtt> _hamqtt;
};