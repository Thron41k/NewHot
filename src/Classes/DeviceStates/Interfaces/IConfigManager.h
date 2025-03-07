// Classes/DeviceStates/Interfaces/IConfigManager.h
#ifndef ICONFIGMANAGER_H
#define ICONFIGMANAGER_H

#include <Classes/DeviceStates/ModeType.h>
#include <string>
#include <IPAddress.h>
class IConfigManager
{
public:
  virtual ModeType GetMode() const = 0;
  virtual void SetMode(ModeType mode) = 0;
  virtual float GetTargetHomeTemp() const = 0;
  virtual void SetTargetHomeTemp(float temp) = 0;
  virtual float GetBoilerMaxTemp() const = 0;
  virtual void SetBoilerMaxTemp(float temp) = 0;
  virtual std::string GetWiFiSSID() const = 0;
  virtual std::string GetWiFiPass() const = 0;
  virtual void SetWiFiCredentials(const char *ssid, const char *pass) = 0;
  virtual IPAddress GetMQTTIP() const = 0;
  virtual void SetMQTTIP(const std::string &ip) = 0;
  virtual int GetMQTTPort() const = 0;
  virtual void SetMQTTPort(int port) = 0;
  virtual std::string GetMQTTUser() const = 0;
  virtual void SetMQTTUser(const std::string &user) = 0;
  virtual std::string GetMQTTPass() const = 0;
  virtual void SetMQTTPass(const std::string &pass) = 0;
  virtual void Loop() = 0;
  virtual ~IConfigManager() = default;
  virtual void SetValvePositionPercent(short percent) = 0;
  virtual short GetValvePositionPercent() const = 0;
  virtual void SetMQTTOutdoorTemperatureTopic(const char *topic) = 0;
  virtual std::string GetMQTTOutdoorTemperatureTopic() const = 0;
  virtual void SetMQTTHomeTemperatureTopic(const char *topic) = 0;
  virtual std::string GetMQTTHomeTemperatureTopic() const = 0;
};

#endif