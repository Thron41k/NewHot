#include "HomeAssistantMQTT.h"

HomeAssistantMQTT::HomeAssistantMQTT(DeviceStates &deviceStates)
    : _deviceStates(&deviceStates)
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    _hadevice = std::make_unique<HADevice>();            
    _hamqtt = std::make_unique<HAMqtt>(client, *_hadevice); 
    _hadevice->setUniqueId(mac, 6);
    _hadevice->setName("Smart boiler");
    _airValve = std::make_unique<HANumber>("AirValve");
    _airValve->setUnitOfMeasurement("%");
    _airValve->setMode(HANumber::ModeSlider);
    _airValve->setMin(0);
    _airValve->setMax(100);
    _airValve->setStep(1);

    _airValve->onCommand([this](HANumeric number, HANumber *sender)
                         { this->onNumberCommand(number, sender); });

    _hamqtt->begin(
        _deviceStates->GetMQTTIP(),
        _deviceStates->GetMQTTUser().c_str(),
        _deviceStates->GetMQTTPass().c_str());
}

void HomeAssistantMQTT::onNumberCommand(HANumeric number, HANumber *sender)
{
    if (!number.isSet())
    {
        return;
    }
    if (sender == _airValve.get())
    {
        _deviceStates->SetValvePercent(number.toInt8());
        return;
    }
}

void HomeAssistantMQTT::Update(ParametreType param_type)
{
    if(param_type == ParametreType::ValvePercent){
        _airValve->setState(_deviceStates->GetValvePercent());
    }
}
