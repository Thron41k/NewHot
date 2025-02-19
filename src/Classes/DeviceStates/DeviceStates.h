#ifndef DEVICESTATES_H
#define DEVICESTATES_H

#include "../BoilerTemperature/BoilerTemperature.h"
#include "../ValveControl/ValveControl.h"
#include <Classes/DeviceConfig/DeviceConfig.h>
#include <Classes/Helpers/Observer/IEncoderEventObserver.h>
#include <Classes/Input/EncoderControl.h>

class DeviceStates : public IObserver, public IEncoderEventObserver
{
private:
    Logger _logger;
    std::unique_ptr<DeviceConfig> _deviceConfig;
    std::unique_ptr<BoilerTemperature> _boilerTemperature;
    std::unique_ptr<ValveControl> _valveControl;
    float _currentHomeTemp = 0.0;
    float _outdoorTemp = 0.0;
    std::vector<IObserver *> observers;
    const EncoderControl *_enc;
    void NotifyObservers(ParametreType param_type) const;

public:
    DeviceStates(Logger logger, const EncoderControl &encoder);
    ~DeviceStates();
    float GetBoilerTemp() const { return _boilerTemperature->GetTemp(); }
    short GetValvePercent() const { return _valveControl->GetPercent(); }
    void SetValvePercent(short percent) const;
    void SetMode(ModeType mode) const;
    ModeType GetMode() const { return _deviceConfig->GetConfig().mode; }
    void SetTargetHomeTemp(float temp) const;
    float GetTargetHomeTemp() const { return _deviceConfig->GetConfig().homeTargetTemp; }
    void SetBoilerMaxTemp(float temp);
    float GetCurrentHomeTemp() const { return _currentHomeTemp; }
    DeviceConfig *GetDeviceConfig() const { return _deviceConfig.get(); }
    float GetOutdoorTemp() const { return _outdoorTemp; }
    void SetWiFiCredentials(const char *ssid, const char *pass) const;
    std::string GetWiFiSSID() const {return _deviceConfig->GetConfig().WIFI_SSID;}
    std::string GetWiFiPass() const {return _deviceConfig->GetConfig().WIFI_PASS;}
    IPAddress GetMQTTIP() const {return _deviceConfig->GetConfig().MQTT_IP;}
    void SetMQTTIP(std::string ip) const;
    int GetMQTTPort() const {return _deviceConfig->GetConfig().MQTT_Port;}
    void SetMQTTPort(int port) const;
    std::string GetMQTTUser() const {return _deviceConfig->GetConfig().MQTT_User;}
    void SetMQTTUser(std::string user) const;
    std::string GetMQTTPass() const {return _deviceConfig->GetConfig().MQTT_Pass;}
    void SetMQTTPass(std::string pass) const;
    void Loop();
    void Update(ParametreType param_type) override;
    void Event(EncoderEventEnum event) override;
    void Attach(IObserver *observer);
    void Detach(IObserver *observer);
};

#endif