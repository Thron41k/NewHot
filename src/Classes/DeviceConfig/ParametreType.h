// Classes/DeviceConfig/ParametreType.h
#ifndef PARAMETRETYPE_H
#define PARAMETRETYPE_H

typedef enum
{
  Mode,
  BoilerMaxTemp,
  HomeTargetTemp,
  ValvePercent,
  HomeCurrentTemp,
  BoilerCurrentTemp,
  WiFiSSID,
  WiFiPass,
  MQTT_IP,
  MQTT_Port,
  MQTT_User,
  MQTT_Pass,
  MQTTHomeTemperatureTopic,
  MQTTOutdoorTemperatureTopic
} ParametreType;

#endif