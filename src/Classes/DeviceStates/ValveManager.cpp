// Classes/DeviceStates/ValveManager.cpp
#include "ValveManager.h"

ValveManager::ValveManager(IConfigManager *configMgr) : _configMgr(configMgr)
{
    _valve = std::make_unique<ValveControl>();
}

void ValveManager::SetPercent(short percent)
{
    _valve->SetPercentage(percent);
    if (_configMgr && _configMgr->GetMode() == ModeType::Manual)
    {
        _configMgr->SetValvePositionPercent(percent);
    }
}
