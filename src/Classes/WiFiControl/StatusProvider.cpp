#include "StatusProvider.h"

const char *StatusProvider::GetStatusJson() const
{
  const char *mode = _wifiManager.IsConnected() ? "STA" : "AP";
  const char *status = _wifiManager.IsConnected() ? "Connected" : "Disconnected";
  const char *ssid = _wifiManager.GetSSID();
  const char *ip = _wifiManager.GetIP();
  snprintf(_jsonBuffer, sizeof(_jsonBuffer),
           "{\"wifi_mode\":\"%s\",\"wifi_status\":\"%s\",\"ssid\":\"%s\",\"ip\":\"%s\"}",
           mode, status, ssid, ip);
  return _jsonBuffer;
}