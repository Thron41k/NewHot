#include "WiFiControl.h"

// Вспомогательная функция для генерации JSON со статусом
String WebServerControl::getStatusJson(const WiFiControl& wifiControl) {
  String json = "{";
  json += "\"wifi_mode\":\"" + String(WiFi.getMode() == WIFI_AP ? "AP" : "STA") + "\",";
  json += "\"wifi_status\":\"" + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected") + "\",";
  json += "\"ssid\":\"" + String(wifiControl.GetDeviceStates()->GetWiFiSSID().c_str()) + "\",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
  json += "}";
  return json;
}

WebServerControl::WebServerControl(WiFiControl &w) 
  : _webserver(std::make_unique<AsyncWebServer>(80)), 
    _dnsserver(std::make_unique<DNSServer>()), 
    _wifiControl(w) 
{
  StartDNS();

  // Главная страница
  _webserver->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String html = R"(
      <!DOCTYPE html>
      <html>
      <head>
        <title>Smart Boiler Setup</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
          body { font-family: Arial, sans-serif; margin: 20px; background: #f4f4f4; color: #333; }
          h1 { color: #2c3e50; }
          .container { max-width: 600px; margin: auto; padding: 20px; background: white; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
          .form-group { margin-bottom: 15px; }
          label { display: block; margin-bottom: 5px; font-weight: bold; }
          select, input[type="text"], input[type="password"] { width: 100%; padding: 8px; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; }
          button { background: #3498db; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; }
          button:hover { background: #2980b9; }
          #status { margin-top: 20px; padding: 10px; border: 1px solid #ddd; border-radius: 4px; background: #ecf0f1; }
        </style>
      </head>
      <body>
        <div class="container">
          <h1>Smart Boiler Setup</h1>
          
          <div class="form-group">
            <h2>Wi-Fi Settings</h2>
            <form id="wifiForm" action="/connect" method="POST">
              <label>Wi-Fi Network:</label>
              <select name="ssid" id="ssidSelect"></select>
              <label>Password:</label>
              <input type="password" name="password" required>
              <button type="submit">Connect</button>
            </form>
          </div>
          
          <div class="form-group">
            <h2>MQTT Settings</h2>
            <form id="mqttForm" action="/mqtt" method="POST">
              <label>Server:</label>
              <input type="text" name="host" value=")";
    html += _wifiControl.GetDeviceStates()->GetMQTTIP().toString();
    html += R"(" required>
              <label>Port:</label>
              <input type="text" name="port" value=")";
    html += String(_wifiControl.GetDeviceStates()->GetMQTTPort());
    html += R"(" required>
              <label>User:</label>
              <input type="text" name="user" value=")";
    html += _wifiControl.GetDeviceStates()->GetMQTTUser().c_str();
    html += R"(">
              <label>Password:</label>
              <input type="password" name="password" value=")";
    html += _wifiControl.GetDeviceStates()->GetMQTTPass().c_str();
    html += R"(">
              <button type="submit">Save</button>
            </form>
          </div>
          
          <div id="status">Status: Loading...</div>
        </div>

        <script>
          function loadNetworks() {
            fetch('/networks')
              .then(response => response.json())
              .then(networks => {
                const select = document.getElementById('ssidSelect');
                select.innerHTML = '';
                networks.forEach(network => {
                  const option = document.createElement('option');
                  option.value = network.ssid;
                  option.text = `${network.ssid} (${network.rssi} dBm)`;
                  select.appendChild(option);
                });
              });
          }

          function updateStatus() {
            fetch('/status')
              .then(response => response.json())
              .then(data => {
                document.getElementById('status').innerHTML = 
                  `Mode: ${data.wifi_mode}<br>Status: ${data.wifi_status}<br>SSID: ${data.ssid}<br>IP: ${data.ip}`;
              });
          }

          document.getElementById('wifiForm').onsubmit = function(e) {
            e.preventDefault();
            fetch('/connect', { method: 'POST', body: new FormData(this) })
              .then(response => response.text())
              .then(text => alert(text))
              .catch(() => alert('Error connecting to Wi-Fi'));
          };

          document.getElementById('mqttForm').onsubmit = function(e) {
            e.preventDefault();
            fetch('/mqtt', { method: 'POST', body: new FormData(this) })
              .then(() => alert('MQTT settings saved'))
              .catch(() => alert('Error saving MQTT settings'));
          };

          loadNetworks();
          updateStatus();
          setInterval(updateStatus, 5000);
        </script>
      </body>
      </html>
    )";
    request->send(200, "text/html", html);
  });

  // Маршрут для списка сетей
  _webserver->on("/networks", HTTP_GET, [this](AsyncWebServerRequest *request) {
    std::vector<NetworkInfo> networks = _wifiControl.ScanNetworks();
    String json = "[";
    for (size_t i = 0; i < networks.size(); i++) {
      json += "{\"ssid\":\"" + String(networks[i].ssid.c_str()) + "\",\"rssi\":" + String(networks[i].rssi) + "}";
      if (i < networks.size() - 1) json += ",";
    }
    json += "]";
    request->send(200, "application/json", json);
  });

  // Маршрут для статуса
  _webserver->on("/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getStatusJson(_wifiControl));
  });

  // Подключение к Wi-Fi
  _webserver->on("/connect", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (!request->hasParam("ssid", true) || !request->hasParam("password", true)) {
      request->send(400, "text/plain", "Invalid request");
      return;
    }
    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();
    bool result = _wifiControl.ConnectToAP(ssid.c_str(), password.c_str());
    if (result) {
      request->send(200, "text/plain", "Connected to " + ssid + ". Rebooting...");
      shouldReboot = true;
    } else {
      request->send(200, "text/plain", "Failed to connect to " + ssid);
    }
  });

  // Сохранение MQTT
  _webserver->on("/mqtt", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (!request->hasParam("host", true) || !request->hasParam("port", true) || 
        !request->hasParam("user", true) || !request->hasParam("password", true)) {
      request->send(400, "text/plain", "Invalid request");
      return;
    }
    _wifiControl.GetDeviceStates()->SetMQTTIP(request->getParam("host", true)->value().c_str());
    _wifiControl.GetDeviceStates()->SetMQTTPort(request->getParam("port", true)->value().toInt());
    _wifiControl.GetDeviceStates()->SetMQTTUser(request->getParam("user", true)->value().c_str());
    _wifiControl.GetDeviceStates()->SetMQTTPass(request->getParam("password", true)->value().c_str());
    request->send(200, "text/plain", "MQTT settings saved");
  });

  _webserver->begin();
}

void WebServerControl::Loop() {
  if (shouldReboot) {
    delay(2000);
    ESP.restart();
  }
  _dnsserver->processNextRequest();
}

void WebServerControl::StartDNS() {
  if (_dns_state) _dnsserver->stop();
  _dns_state = _dnsserver->start(53, "*", WiFi.softAPIP());
}

void WebServerControl::StopDNS() {
  if (_dns_state) _dnsserver->stop();
  _dns_state = false;
}