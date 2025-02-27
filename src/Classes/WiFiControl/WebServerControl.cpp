#include "WebServerControl.h"

WebServerControl::WebServerControl(IWiFiManager &wifiManager, IConfigManager &configMgr, StatusProvider statusProvider)
    : _webserver(std::make_unique<AsyncWebServer>(80)), _dnsserver(std::make_unique<DNSServer>()),
      _wifiManager(wifiManager), _configMgr(configMgr), _statusProvider(std::move(statusProvider))
{
}

void WebServerControl::init()
{
  setupRoutes();
  _webserver->begin();
}

void WebServerControl::setupRoutes()
{
  _webserver->on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
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
          #networksList { margin-top: 10px; }
        </style>
      </head>
      <body>
        <div class="container">
          <h1>Smart Boiler Setup</h1>

          <div class="form-group">
            <h2>Wi-Fi Settings</h2>
            <button onclick=\"scanNetworks()\">Scan Networks</button>
            <div id="networksList"></div>
            <form id="wifiForm">
              <label>Wi-Fi Network:</label>
              <select name="ssid" id="ssidSelect"></select>
              <label>Password:</label>
              <input type="password" name="password" required>
              <button type="submit">Connect</button>
            </form>
          </div>

          <div class="form-group">
            <h2>MQTT Settings</h2>
            <form id="mqttForm">
              <label>Server:</label>
              <input type="text" name="host" value=")" + _configMgr.GetMQTTIP().toString() + R"(" required>
              <label>Port:</label>
              <input type="text" name="port" value=")" + String(_configMgr.GetMQTTPort()) + R"(" required>
              <label>User:</label>
              <input type="text" name="user" value=")" + String(_configMgr.GetMQTTUser().c_str()) + R"(">
              <label>Password:</label>
              <input type="password" name="password" value=")" + String(_configMgr.GetMQTTPass().c_str()) + R"(">
              <button type="submit">Save</button>
            </form>
          </div>

          <div id="status">Status: Loading...</div>
        </div>

        <script>
          function scanNetworks() {
            fetch('/networks')
              .then(response => response.json())
              .then(networks => {
                const select = document.getElementById('ssidSelect');
                const list = document.getElementById('networksList');
                select.innerHTML = '';
                list.innerHTML = 'Available Networks:<br>';
                networks.forEach(network => {
                  const option = document.createElement('option');
                  option.value = network.ssid;
                  option.text = `${network.ssid} (${network.rssi} dBm)`;
                  select.appendChild(option);
                  list.innerHTML += `${network.ssid} (${network.rssi} dBm)<br>`;
                });
              })
              .catch(() => alert('Error scanning networks'));
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
            fetch('/connect', { 
              method: 'POST', 
              body: new FormData(this) 
            })
            .then(response => response.text())
            .then(text => {
              alert(text);
              if (text.includes('Rebooting')) setTimeout(() => updateStatus(), 3000);
            })
            .catch(() => alert('Error connecting to Wi-Fi'));
          };

          document.getElementById('mqttForm').onsubmit = function(e) {
            e.preventDefault();
            fetch('/mqtt', { 
              method: 'POST', 
              body: new FormData(this) 
            })
            .then(() => alert('MQTT settings saved'))
            .then(() => updateStatus())
            .catch(() => alert('Error saving MQTT settings'));
          };

          scanNetworks();
          updateStatus();
          setInterval(updateStatus, 5000);
        </script>
      </body>
      </html>
    )";
    request->send(200, "text/html", html); });

  _webserver->on("/networks", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
    std::vector<NetworkInfo> networks = _wifiManager.ScanNetworks();
    String json = "[";
    for (size_t i = 0; i < networks.size(); i++) {
      json += "{\"ssid\":\"" + String(networks[i].ssid.c_str()) + "\",\"rssi\":" + String(networks[i].rssi) + "}";
      if (i < networks.size() - 1) json += ",";
    }
    json += "]";
    request->send(200, "application/json", json); });

  _webserver->on("/status", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
      Serial.println("Handling /status request");
      Serial.print("Free heap before GetStatusJson: ");
      Serial.println(ESP.getFreeHeap());
  
      // Проверка целостности
      if (this == nullptr) {
        Serial.println("WebServerControl this is null!");
        request->send(500, "text/plain", "Internal error: null this");
        return;
      }
  
      Serial.println("Calling GetStatusJson");
      const char* json = _statusProvider.GetStatusJson();
      Serial.println("Got JSON from StatusProvider: ");
      Serial.println(json);
  
      request->send(200, "application/json", json);
      Serial.println("Sent /status response"); });
  _webserver->on("/connect", HTTP_POST, [this](AsyncWebServerRequest *request)
                 {
    if (!request->hasParam("ssid", true) || !request->hasParam("password", true)) {
      request->send(400, "text/plain", "Invalid request");
      return;
    }
    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();
    bool result = _wifiManager.ConnectToAP(ssid.c_str(), password.c_str());
    if (result) {
      request->send(200, "text/plain", "Connected to " + ssid + ". Rebooting...");
      _shouldReboot = true;
    } else {
      request->send(200, "text/plain", "Failed to connect to " + ssid);
    } });

  _webserver->on("/mqtt", HTTP_POST, [this](AsyncWebServerRequest *request)
                 {
    if (!request->hasParam("host", true) || !request->hasParam("port", true) || 
        !request->hasParam("user", true) || !request->hasParam("password", true)) {
      request->send(400, "text/plain", "Invalid request");
      return;
    }
    _configMgr.SetMQTTIP(request->getParam("host", true)->value().c_str());
    _configMgr.SetMQTTPort(request->getParam("port", true)->value().toInt());
    _configMgr.SetMQTTUser(request->getParam("user", true)->value().c_str());
    _configMgr.SetMQTTPass(request->getParam("password", true)->value().c_str());
    request->send(200, "text/plain", "MQTT settings saved"); });
}

void WebServerControl::Loop()
{
  if (_shouldReboot)
  {
    delay(2000);
    ESP.restart();
  }
  if (_wifiManager.isWifiReady() && _dnsState)
  { // Проверяем состояние DNS
    Serial.println("DNS state: 1");
    _dnsserver->processNextRequest();
    Serial.println("DNS state: 2");
  }
}

void WebServerControl::StartDNS()
{
  if (!_wifiManager.isWifiReady())
  {
    Serial.println("WiFi not ready, skipping DNS start");
    return;
  }
  if (_dnsState)
  {
    Serial.println("DNS already running, stopping first");
    _dnsserver->stop();
  }
  _dnsState = _dnsserver->start(53, "*", WiFi.softAPIP());
  Serial.println("DNS server started: " + String(_dnsState ? "success" : "failed"));
}

void WebServerControl::StopDNS()
{
  if (_dnsState)
  {
    _dnsserver->stop();
    _dnsState = false;
    Serial.println("DNS server stopped");
  }
}