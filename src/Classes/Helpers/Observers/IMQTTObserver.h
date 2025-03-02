#ifndef IMQTTOBSERVER_H
#define IMQTTOBSERVER_H

#include <cstdint>
class IMQTTObserver {
    public:
      virtual void MQTTEvent(const char* topic, const uint8_t* payload, uint16_t length) = 0;
      virtual ~IMQTTObserver() = default;
    };

#endif