#include "ValveManager.h"

ValveManager::ValveManager()
{
    Serial.println("ValveManager initialized 0");
    _valve = std::make_unique<ValveControl>();
    Serial.println("ValveManager initialized 1");
}