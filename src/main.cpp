#include <Arduino.h>
#include "Classes/Helpers/Logger.h"
#include "Classes/Input/EncoderControl.h"
#include "Classes/DeviceStates/DeviceStates.h"
#include "Classes/WiFiControl/WiFiControl.h"
#include "Classes/HomeAssistantMQTT/HomeAssistantMQTT.h"
#include "Classes/DeviceStates/TemperatureManager.h"
#include "Classes/DeviceStates/ValveManager.h"
#include "Classes/DeviceStates/ConfigManager.h"
#include "Classes/Display/LcdDisplayRenderer.h"
#include "Classes/Display/DisplayController.h"

void setup() {
  Serial.begin(115200);
  delay(100);

  Logger logger;
  EncoderControl enc;
  auto tempMgr = std::make_unique<TemperatureManager>(logger); // Использует TEMPERATURE_SENSOR по умолчанию
  // Или явно указать пин: auto tempMgr = std::make_unique<TemperatureManager>(logger, 4);
  auto valveMgr = std::make_unique<ValveManager>();
  auto configMgr = std::make_unique<ConfigManager>();
  DeviceStates deviceStates(std::move(tempMgr), std::move(valveMgr), std::move(configMgr), enc);
  
  auto renderer = std::make_unique<LcdDisplayRenderer>();
  DisplayController display(std::move(renderer), *tempMgr, *valveMgr, *configMgr);
  
  deviceStates.Attach(&display);
  valveMgr->Attach(&display);
  configMgr->Attach(&display);

  WiFiControl wifi(*configMgr);
  HomeAssistantMQTT haMqtt(*valveMgr, *configMgr);

  logger.Log("System initialized");

  while (true) {
    enc.Loop();
    deviceStates.Loop();
    display.Loop();
    wifi.Loop();
    delay(10);
  }
}

void loop() {}

