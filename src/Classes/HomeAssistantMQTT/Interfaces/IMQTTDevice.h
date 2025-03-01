#ifndef IMQTTDEVICE_H
#define IMQTTDEVICE_H

#include <HADevice.h>
#include <HAMqtt.h>

class IMQTTDevice {
public:
    virtual void setup(HADevice& device, HAMqtt& mqtt) = 0;
    virtual void loop() = 0;
    virtual void Set(short percent) = 0;
    virtual void Report() = 0;
    virtual ~IMQTTDevice() = default;
};

#endif