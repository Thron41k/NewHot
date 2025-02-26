#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Classes/DeviceStates/ModeType.h>
#include <IPAddress.h>

struct Configuration {
    ModeType mode = ModeType::Manual;
    float homeTargetTemp = 25.5;
    short valvePercent = 10;
    float boilerMaxTemp = 50.0;
    char WIFI_SSID[32];
    char WIFI_PASS[32];
    char MQTT_User[32];
    char MQTT_Pass[32];
    int MQTT_Port = 1883;
    IPAddress MQTT_IP = {0, 0, 0, 0};
  };

#endif