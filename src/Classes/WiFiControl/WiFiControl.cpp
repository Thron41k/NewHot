#include "WiFiControl.h"

WiFiControl::WiFiControl(IConfigManager &configMgr, std::unique_ptr<IWiFiStrategy> strategy)
    : _configMgr(configMgr), _strategy(std::move(strategy)), _lastConnection(millis())
{
  strcpy(_ssidBuffer, "N/A");
  strcpy(_ipBuffer, "0.0.0.0");
  strncpy(_apSSID, AP_WIFI_SSID, sizeof(_apSSID) - 1);
  _apSSID[sizeof(_apSSID) - 1] = '\0';
  strncpy(_apPassword, AP_WIFI_PASSWORD, sizeof(_apPassword) - 1);
  _apPassword[sizeof(_apPassword) - 1] = '\0';
}

void WiFiControl::init()
{
  _wifiInitialized = true;
  Serial.println("WiFi initialized");

  std::string ssid = _configMgr.GetWiFiSSID();
  std::string password = _configMgr.GetWiFiPass();
  if (!ssid.empty() && !password.empty())
  {
    ConnectToAP(ssid, password);
  }
  else
  {
    _strategy->StartAP();
    if (_webServer)
      _webServer->StartDNS();
    strncpy(_ssidBuffer, _apSSID, sizeof(_ssidBuffer) - 1);
    _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
    strncpy(_ipBuffer, "192.168.4.1", sizeof(_ipBuffer) - 1);
    _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
  }
}

void WiFiControl::setWebServer(std::unique_ptr<IWebServer> webServer)
{
  _webServer = std::move(webServer);
  if (_webServer && _wifiInitialized && WiFi.getMode() == WIFI_AP)
  {
    _webServer->StartDNS();
  }
}

bool WiFiControl::ConnectToAP(const std::string &ssid, const std::string &password)
{
  if (!_wifiInitialized)
  {
    Serial.println("WiFi not initialized, skipping connect");
    return false;
  }
  if (_strategy->Connect(ssid, password))
  {
    _configMgr.SetWiFiCredentials(ssid.c_str(), password.c_str());
    if (_webServer)
      _webServer->StopDNS();
    strncpy(_ssidBuffer, ssid.c_str(), sizeof(_ssidBuffer) - 1);
    _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
    strncpy(_ipBuffer, WiFi.localIP().toString().c_str(), sizeof(_ipBuffer) - 1);
    _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
    return true;
  }
  else
  {
    _strategy->StartAP();
    if (_webServer)
      _webServer->StartDNS();
    strncpy(_ssidBuffer, _apSSID, sizeof(_ssidBuffer) - 1);
    _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
    strncpy(_ipBuffer, "192.168.4.1", sizeof(_ipBuffer) - 1);
    _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
    return false;
  }
}

const char *WiFiControl::GetSSID() const
{
  if (!_wifiInitialized)
    return "N/A";
  return _ssidBuffer;
}

const char *WiFiControl::GetIP() const
{
  if (!_wifiInitialized)
    return "0.0.0.0";
  return _ipBuffer;
}

std::vector<NetworkInfo> WiFiControl::ScanNetworks()
{
  if (!_wifiInitialized)
  {
    Serial.println("WiFi not initialized, skipping scan");
    return {};
  }
  if (_scanInProgress && isScanComplete())
  {
    return _scanResults;
  }
  startScan();
  return {};
}

void WiFiControl::startScan()
{
  if (!_wifiInitialized)
  {
    Serial.println("WiFi not initialized, skipping scan");
    return;
  }
  if (!_scanInProgress)
  {
    _scanResults.clear();
    _wasAP = (WiFi.getMode() == WIFI_AP);
    if (_wasAP)
    {
      Serial.println("Stopping AP for scanning");
      //WiFi.softAPdisconnect();
      delay(1000);
      WiFi.mode(WIFI_STA);
      delay(1000);
      Serial.print("WiFi mode after switch: ");
      Serial.println(WiFi.getMode());
    }
    // Проверяем готовность перед сканированием
    int attempt = 0;
    const int maxAttempts = 3;
    int scanResult;
    do
    {
      scanResult = WiFi.scanNetworks(true);
      if (scanResult == WIFI_SCAN_FAILED)
      {
        Serial.println("Scan attempt " + String(attempt + 1) + " failed, retrying");
        delay(100);
        attempt++;
      }
      else
      {
        break;
      }
    } while (attempt < maxAttempts);

    if (scanResult == WIFI_SCAN_FAILED)
    {
      Serial.println("All scan attempts failed");
      _scanInProgress = false; // Сбрасываем флаг, чтобы не ждать
      if (_wasAP)
        restoreAP();
      return;
    }

    _scanInProgress = true;
    Serial.println("Scan started");
  }
}

bool WiFiControl::isScanComplete() const
{
  return _scanInProgress && WiFi.scanComplete() != WIFI_SCAN_RUNNING;
}

std::vector<NetworkInfo> WiFiControl::getScanResults() const
{
  if (!_scanInProgress || WiFi.scanComplete() == WIFI_SCAN_RUNNING)
  {
    return std::vector<NetworkInfo>();
  }
  return _scanResults;
}

void WiFiControl::Loop()
{
  if (_webServer && _wifiInitialized)
    _webServer->Loop();
  if (_wifiInitialized && millis() - _lastConnection >= WIFI_RECONNECTION_TIMEOUT && WiFi.getMode() == WIFI_AP)
  {
    _lastConnection = millis();
    ConnectToAP(_configMgr.GetWiFiSSID(), _configMgr.GetWiFiPass());
  }
  if (_scanInProgress)
  {
    int scanStatus = WiFi.scanComplete();
    if (scanStatus != WIFI_SCAN_RUNNING)
    {
      Serial.println("Starting in progress 2");
      if (scanStatus >= 0)
      {
        Serial.println("Starting in progress 3");
        _scanResults.clear();
        for (int i = 0; i < scanStatus; ++i)
        {
          NetworkInfo network;
          network.ssid = WiFi.SSID(i).c_str();
          network.rssi = WiFi.RSSI(i);
          _scanResults.push_back(network);
        }
        Serial.println("Scan completed, found " + String(scanStatus) + " networks");
      }
      else
      {
        Serial.println("Scan failed with status: " + String(scanStatus));
        // Даем больше времени для завершения (10 секунд)
        if (millis() - _lastConnection < 10000)
        {
          delay(100);
          return;
        }
      }
      _scanInProgress = false;
      WiFi.scanDelete();
      if (_wasAP)
      {
        restoreAP();
      }
    }
  }
}

void WiFiControl::restoreAP()
{
  Serial.println("Restoring AP mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(_apSSID, _apPassword);
  if (_webServer)
    _webServer->StartDNS();
  strncpy(_ssidBuffer, _apSSID, sizeof(_ssidBuffer) - 1);
  _ssidBuffer[sizeof(_ssidBuffer) - 1] = '\0';
  strncpy(_ipBuffer, "192.168.4.1", sizeof(_ipBuffer) - 1);
  _ipBuffer[sizeof(_ipBuffer) - 1] = '\0';
  _wasAP = false;
  Serial.println("AP restored");
}

bool WiFiControl::isWifiReady() const
{
  return _wifiInitialized;
}