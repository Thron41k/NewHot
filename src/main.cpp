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
#include "Classes/WiFiControl/StatusProvider.h"
#include "Classes/WiFiControl/WebServerControl.h"

// Глобальные объекты (объявление без инициализации)
Logger logger;
EncoderControl enc;
std::unique_ptr<TemperatureManager> tempMgr;
std::unique_ptr<ValveManager> valveMgr;
std::unique_ptr<ConfigManager> configMgr;
DeviceStates *deviceStates = nullptr;
std::unique_ptr<LcdDisplayRenderer> renderer;
DisplayController *display = nullptr;
WiFiControl *wifi = nullptr;
HomeAssistantMQTT *haMqtt = nullptr;

void setup()
{
  Serial.begin(115200);
  delay(100);
  logger.Log("Starting setup");

  tempMgr = std::make_unique<TemperatureManager>(logger);
  delay(10);
  logger.Log("TemperatureManager initialized");

  valveMgr = std::make_unique<ValveManager>();
  delay(10);
  logger.Log("ValveManager initialized");

  configMgr = std::make_unique<ConfigManager>();
  configMgr->init();
  delay(10);
  logger.Log("ConfigManager initialized");

  deviceStates = new DeviceStates(std::move(tempMgr), std::move(valveMgr), std::move(configMgr), enc);
  delay(10);
  logger.Log("DeviceStates initialized");

  renderer = std::make_unique<LcdDisplayRenderer>();
  delay(10);
  logger.Log("Renderer initialized");

  display = new DisplayController(std::move(renderer), *deviceStates->getTempMgr(), *deviceStates->getValveMgr(), *deviceStates->getConfigMgr());
  delay(10);
  logger.Log("DisplayController initialized");

  wifi = new WiFiControl(*deviceStates->getConfigMgr(), std::make_unique<WiFiSTAStrategy>());
  wifi->init();
  delay(10);
  logger.Log("WiFiControl initialized");
  StatusProvider provider(*wifi);
  Serial.println("Creating WebServerControl");
  std::unique_ptr<WebServerControl> webServer = std::make_unique<WebServerControl>(*wifi, *deviceStates->getConfigMgr(), provider);
  webServer->init();
  wifi->setWebServer(std::move(webServer));
  delay(10);
  logger.Log("WebServerControl added to WiFiControl");

  deviceStates->Attach(static_cast<ITemperatureObserver *>(display));
  deviceStates->Attach(static_cast<IValveObserver *>(display));
  deviceStates->Attach(static_cast<IConfigObserver *>(display));
  logger.Log("Observers attached");

  logger.Log("System initialized");
}

void loop()
{
  if (!wifi->IsConnected())
  {
    // logger.Error("WiFi disconnected");
  }

  enc.Loop();
  deviceStates->Loop();
  display->Loop();
  wifi->Loop();
  if (wifi->IsConnected() && haMqtt == nullptr)
  {
    logger.Log("WiFi connected in STA mode, initializing HomeAssistantMQTT");
    uint8_t mac[6];
    WiFi.macAddress(mac);
    std::string uniqueId(reinterpret_cast<char *>(mac), 6);
    haMqtt = new HomeAssistantMQTT("Smart Boiler", uniqueId);
    haMqtt->addDevice(std::make_unique<MQTTValveDevice>(*deviceStates->getValveMgr()));
    haMqtt->begin(
        deviceStates->getConfigMgr()->GetMQTTIP().toString().c_str(),
        deviceStates->getConfigMgr()->GetMQTTUser(),
        deviceStates->getConfigMgr()->GetMQTTPass());
    logger.Log("HomeAssistantMQTT initialized");
  }

  // Выполняем Loop для haMqtt только после его инициализации
  if (haMqtt)
  {
    haMqtt->loop();
  }

  delay(10);
}