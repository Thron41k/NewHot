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
#include "Classes/WiFiControl/WiFiSTAStrategy.h"
#include "Classes/WiFiControl/WebServerControl.h"
#include "Classes/WiFiControl/StatusProvider.h"

void setup()
{
  Serial.begin(115200);
  delay(100);
  Logger logger;
  EncoderControl enc;
  logger.Log("System initialized 0");
  auto tempMgr = std::make_unique<TemperatureManager>(logger);
  logger.Log("System initialized 1");
  auto valveMgr = std::make_unique<ValveManager>();
  logger.Log("System initialized 2");
  auto configMgr = std::make_unique<ConfigManager>();
  logger.Log("System initialized 3");
  DeviceStates deviceStates(std::move(tempMgr), std::move(valveMgr), std::move(configMgr), enc);
  auto renderer = std::make_unique<LcdDisplayRenderer>();
  DisplayController display(std::move(renderer), *deviceStates.getTempMgr(), *deviceStates.getValveMgr(), *deviceStates.getConfigMgr());
  WiFiControl wifi(
      *deviceStates.getConfigMgr(),
      std::make_unique<WiFiSTAStrategy>(),
      std::make_unique<WebServerControl>(wifi, *deviceStates.getConfigMgr(), StatusProvider(wifi)));
  logger.Log("System initialized 1");
  // Привязка наблюдателей к DeviceStates
  /*deviceStates.Attach(static_cast<ITemperatureObserver *>(&display));
  deviceStates.Attach(static_cast<IValveObserver *>(&display));
  deviceStates.Attach(static_cast<IConfigObserver *>(&display));
  logger.Log("System initialized 2");
  // Установка уникального ID на основе MAC-адреса
  uint8_t mac[6];
  WiFi.macAddress(mac);
  std::string uniqueId(reinterpret_cast<char *>(mac), 6);
  HomeAssistantMQTT haMqtt = HomeAssistantMQTT("Smart Boiler", uniqueId);
  logger.Log("System initialized 3");
  // Добавление устройства MQTT для клапана
  haMqtt.addDevice(std::make_unique<MQTTValveDevice>(*deviceStates.getValveMgr()));
  haMqtt.begin(configMgr->GetMQTTIP().toString().c_str(), configMgr->GetMQTTUser(), configMgr->GetMQTTPass());
  logger.Log("System initialized");*/
}

void loop()
{
  // Обработка ошибок подключения
  /*if (!wifi.IsConnected())
  {
    logger.Error("WiFi disconnected");
  }

  // Выполнение основных циклов
  enc.Loop();
  deviceStates.Loop();
  display.Loop();
  wifi.Loop();
  haMqtt.loop();

  delay(10); // Небольшая задержка для стабильности*/
}