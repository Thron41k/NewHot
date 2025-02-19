#include "DeviceStates.h"

void DeviceStates::NotifyObservers(ParametreType param_type) const
{
    for (auto observer : observers)
    {
        observer->Update(param_type);
    }
}

DeviceStates::DeviceStates(Logger logger, const EncoderControl &encoder)
    : _enc(&encoder), _boilerTemperature(std::make_unique<BoilerTemperature>(logger)), _valveControl(std::make_unique<ValveControl>()), _deviceConfig(std::make_unique<DeviceConfig>())
{
    const_cast<BoilerTemperature &>(*_boilerTemperature).Attach(this);
    const_cast<EncoderControl &>(*_enc).Attach(this);
}

DeviceStates::~DeviceStates()
{
    const_cast<BoilerTemperature &>(*_boilerTemperature).Detach(this);
    const_cast<EncoderControl &>(*_enc).Detach(this);
}

void DeviceStates::SetValvePercent(short percent) const
{
    percent = std::clamp(percent, static_cast<short>(0), static_cast<short>(100));
    _valveControl->SetPercentage(percent);
    _deviceConfig->SetParametre(percent, ValvePercent);
    if (_deviceConfig->GetConfig().mode == ModeType::Manual)
    {
        NotifyObservers(ParametreType::ValvePercent);
    }
}

void DeviceStates::SetMode(ModeType mode) const
{
    _deviceConfig->SetParametre(mode, Mode);
    NotifyObservers(ParametreType::Mode);
}

void DeviceStates::SetTargetHomeTemp(float temp) const
{
    _deviceConfig->SetParametre(temp, HomeTargetTemp);
    if (_deviceConfig->GetConfig().mode == ModeType::Auto)
    {
        NotifyObservers(ParametreType::HomeTargetTemp);
    }
}

void DeviceStates::SetBoilerMaxTemp(float temp)
{
    _deviceConfig->SetParametre(temp, BoilerMaxTemp);
    NotifyObservers(ParametreType::BoilerMaxTemp);
}

void DeviceStates::SetWiFiCredentials(const char *ssid, const char *pass) const
{
    _deviceConfig->SetParametre(ssid, WiFiSSID);
    _deviceConfig->SetParametre(pass, WiFiPass);
}

void DeviceStates::SetMQTTIP(std::string ip)
{
    _deviceConfig->SetParametre(ip, MQTT_IP);
}

void DeviceStates::SetMQTTPort(int port)
{
    _deviceConfig->SetParametre(port, MQTT_Port);
}

void DeviceStates::SetMQTTUser(std::string user)
{
    _deviceConfig->SetParametre(user, MQTT_User);
}

void DeviceStates::SetMQTTPass(std::string pass)
{
    _deviceConfig->SetParametre(pass, MQTT_Pass);
}

void DeviceStates::Loop()
{
    _boilerTemperature->Loop();
    _deviceConfig->Loop();
}

void DeviceStates::Update(ParametreType param_type)
{
    NotifyObservers(param_type);
}

void DeviceStates::Event(EncoderEventEnum event)
{
}

void DeviceStates::Attach(IObserver *observer)
{
    observers.push_back(observer);
}

void DeviceStates::Detach(IObserver *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}
