#ifndef MQTTVALVEDEVICE_H
#define MQTTVALVEDEVICE_H

#include "Interfaces/IMQTTDevice.h"
#include <Classes/DeviceStates/Interfaces/IValveManager.h>
#include <device-types/HANumber.h>
#include <memory>

class MQTTValveDevice : public IMQTTDevice {
private:
    IValveManager& _valveManager;
    std::unique_ptr<HANumber> _airValve;

public:
    MQTTValveDevice(IValveManager& valveManager) : _valveManager(valveManager) {}

    void setup(HADevice& device, HAMqtt& mqtt) override {
        _airValve = std::make_unique<HANumber>("AirValve");
        _airValve->setUnitOfMeasurement("%");
        _airValve->setMode(HANumber::ModeSlider);
        _airValve->setMin(0);
        _airValve->setMax(100);
        _airValve->setStep(1);

        _airValve->onCommand([this](HANumeric number, HANumber* sender) {
            if (number.isSet()) {
                _valveManager.SetPercent(number.toInt8());
                sender->setState(number.toInt8());
            }
        });
    }
    void Set(short percent)  override {
        _airValve->setState(percent);
    }

    void loop() override {
       
    }
};

#endif