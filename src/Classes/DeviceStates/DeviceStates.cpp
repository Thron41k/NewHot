#include "DeviceStates.h"

void DeviceStates::SetValvePercent(short percent)
{
    percent = std::clamp(percent, static_cast<short>(0), static_cast<short>(100));
    _valveControl->SetPercentage(percent);
    _deviceConfig->SetParametre(percent, ValvePercent);
}

void DeviceStates::SetMode(ModeType mode)
{
    _deviceConfig->SetParametre(mode, Mode);
}

void DeviceStates::SetHomeTargetTemp(float temp)
{
    _deviceConfig->SetParametre(temp, HomeTargetTemp);
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
