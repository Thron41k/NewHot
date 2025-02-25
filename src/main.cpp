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
#include "Classes/HomeAssistantMQTT/MQTTValveDevice.h"

void setup()
{
  Serial.begin(115200);
  delay(100);

  Logger logger;
  EncoderControl enc;
  auto tempMgr = std::make_unique<TemperatureManager>(logger);
  auto valveMgr = std::make_unique<ValveManager>();
  auto configMgr = std::make_unique<ConfigManager>();
  auto mqttValveDevice = std::make_unique<MQTTValveDevice>(*valveMgr);
  DeviceStates deviceStates(std::move(tempMgr), std::move(valveMgr), std::move(configMgr), enc);

  auto renderer = std::make_unique<LcdDisplayRenderer>();
  DisplayController display(std::move(renderer), *tempMgr, *valveMgr, *configMgr);

  deviceStates.Attach(static_cast<ITemperatureObserver *>(&display));
  deviceStates.Attach(static_cast<IValveObserver *>(&display));
  deviceStates.Attach(static_cast<IConfigObserver *>(&display));

  WiFiControl wifi(*configMgr);

  uint8_t mac[6];
  WiFi.macAddress(mac);
  std::string uniqueId(reinterpret_cast<char *>(mac), 6);
  HomeAssistantMQTT haMqtt("Smart Boiler", uniqueId);
  haMqtt.addDevice(std::move(mqttValveDevice));
  haMqtt.begin(configMgr->GetMQTTIP().toString(), configMgr->GetMQTTUser(), configMgr->GetMQTTPass());
  logger.Log("System initialized");

  while (true)
  {
    enc.Loop();
    deviceStates.Loop();
    display.Loop();
    wifi.Loop();
    delay(10);
  }
}

void loop() {}
