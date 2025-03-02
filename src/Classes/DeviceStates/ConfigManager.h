#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "Classes/DeviceStates/Interfaces/IConfigManager.h"
#include "Classes/DeviceConfig/DeviceConfig.h"
class ConfigManager : public IConfigManager {
private:
  std::unique_ptr<DeviceConfig> _config;
public:
  ConfigManager(){};
  void init();
  ModeType GetMode() const override { return _config->GetConfig().mode; }
  void SetMode(ModeType mode) override { _config->SetParametre(mode, Mode); }
  float GetTargetHomeTemp() const override { return _config->GetConfig().homeTargetTemp; }
  void SetTargetHomeTemp(float temp) override { _config->SetParametre(temp, HomeTargetTemp); }
  float GetBoilerMaxTemp() const override { return _config->GetConfig().boilerMaxTemp; }
  void SetBoilerMaxTemp(float temp) override { _config->SetParametre(temp, BoilerMaxTemp); }
  std::string GetWiFiSSID() const override { return _config->GetConfig().WIFI_SSID; }
  std::string GetWiFiPass() const override { return _config->GetConfig().WIFI_PASS; }
  void SetWiFiCredentials(const char* ssid, const char* pass) override {
    _config->SetParametre(ssid, WiFiSSID);
    _config->SetParametre(pass, WiFiPass);
  }
  IPAddress GetMQTTIP() const override { 
    return _config->GetConfig().MQTT_IP;
   }
  void SetMQTTIP(const std::string& ip) override { _config->SetParametre(ip, MQTT_IP); }
  int GetMQTTPort() const override { return _config->GetConfig().MQTT_Port; }
  void SetMQTTPort(int port) override { _config->SetParametre(port, MQTT_Port); }
  std::string GetMQTTUser() const override { return _config->GetConfig().MQTT_User; }
  void SetMQTTUser(const std::string& user) override { _config->SetParametre(user, MQTT_User); }
  std::string GetMQTTPass() const override { return _config->GetConfig().MQTT_Pass; }
  void SetMQTTPass(const std::string& pass) override { _config->SetParametre(pass, MQTT_Pass); }
  void Loop() override { _config->Loop(); }
  void SetValvePositionPercent(short percent) override { _config->SetParametre(percent, ValvePercent); }
  short GetValvePositionPercent() const override { return _config->GetConfig().valvePercent; }
  void SetMQTTOutdoorTemperatureTopic(const char* topic) override { _config->SetParametre(topic, MQTTOutdoorTemperatureTopic); }
  void SetMQTTHomeTemperatureTopic(const char* topic) override { _config->SetParametre(topic, MQTTHomeTemperatureTopic); }
  std::string GetMQTTOutdoorTemperatureTopic() const override { return _config->GetConfig().MQTTOutdoorTemperatureTopic; }
  std::string GetMQTTHomeTemperatureTopic() const override { return _config->GetConfig().MQTTHomeTemperatureTopic; }
};

#endif