#include "DeviceStates.h"

void DeviceStates::NotifyObservers()
{
    for (auto observer : observers)
    {
        observer->Update();
    }
}

void DeviceStates::SetValvePercent(short percent)
{
    percent = std::clamp(percent, static_cast<short>(0), static_cast<short>(100));
    _valveControl->SetPercentage(percent);
    _deviceConfig->SetParametre(percent, ValvePercent);
    if (_deviceConfig->GetConfig().mode == ModeType::Manual){
        NotifyObservers();
    }
}

void DeviceStates::SetMode(ModeType mode)
{
    _deviceConfig->SetParametre(mode, Mode);
    NotifyObservers();
}

void DeviceStates::SetHomeTargetTemp(float temp)
{
    _deviceConfig->SetParametre(temp, HomeTargetTemp);
    if(_deviceConfig->GetConfig().mode == ModeType::Auto){
        NotifyObservers();
    }
}

void DeviceStates::SetBoilerMaxTemp(float temp)
{
    _deviceConfig->SetParametre(temp, BoilerMaxTemp);
}

void DeviceStates::Loop()
{
    _boilerTemperature->Loop();
    _deviceConfig->Loop();
}

void DeviceStates::Update()
{
    NotifyObservers();
}

void DeviceStates::Attach(IObserver *observer)
{
    observers.push_back(observer);
}

void DeviceStates::Detach(IObserver *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}
