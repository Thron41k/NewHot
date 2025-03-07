// Classes/DeviceStates/ConfigManager.cpp
#include "ConfigManager.h"

void ConfigManager::init()
{
  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount LittleFS");
    // Можно добавить форматирование как fallback
    if (!LittleFS.format())
    {
      Serial.println("Failed to format LittleFS");
    }
    else if (!LittleFS.begin())
    {
      Serial.println("LittleFS mount failed after formatting");
    }
    else
    {
      Serial.println("LittleFS mounted after formatting");
    }
  }
  else
  {
    Serial.println("LittleFS mounted successfully");
  }
  _config = std::make_unique<DeviceConfig>();
}