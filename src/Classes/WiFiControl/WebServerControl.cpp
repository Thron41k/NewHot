// WebServerControl.cpp
#include "WebServerControl.h"
#include <Update.h>

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
          body { font-family: Arial, sans-serif; margin: 0; background: #f4f4f4; color: #333; }
          .container { max-width: 600px; margin: 20px auto; padding: 20px; background: white; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
          h1 { color: #2c3e50; text-align: center; }
          .tabs { overflow: hidden; border-bottom: 1px solid #ccc; }
          .tab-button { background: #f1f1f1; border: none; outline: none; cursor: pointer; padding: 10px 20px; float: left; width: 50%; text-align: center; transition: 0.3s; }
          .tab-button:hover { background: #ddd; }
          .tab-button.active { background: #ccc; }
          .tab-content { display: none; padding: 20px 0; }
          .tab-content.active { display: block; }
          .accordion { background: #f9f9f9; border: 1px solid #ddd; border-radius: 4px; margin-bottom: 10px; }
          .accordion-header { background: #e9e9e9; padding: 10px; cursor: pointer; font-weight: bold; }
          .accordion-content { display: none; padding: 10px; }
          .accordion-content.active { display: block; }
          label { display: block; margin-bottom: 5px; font-weight: bold; }
          select, input[type="text"], input[type="password"], input[type="file"] { width: 100%; padding: 8px; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; margin-bottom: 10px; }
          button { background: #3498db; color: white; padding: 10px 15px; border: none; border-radius: 4px; cursor: pointer; }
          button:hover { background: #2980b9; }
          button:disabled { background: #cccccc; cursor: not-allowed; }
          #status { padding: 10px; border: 1px solid #ddd; border-radius: 4px; background: #ecf0f1; }
          #rebootOverlay { display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(0,0,0,0.5); z-index: 1000; }
          #rebootIndicator { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); text-align: center; color: white; }
          .spinner { border: 4px solid #f3f3f3; border-top: 4px solid #3498db; border-radius: 50%; width: 40px; height: 40px; animation: spin 1s linear infinite; margin: 0 auto; }
          @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        </style>
      </head>
      <body>
        <div class="container">
          <h1>Smart Boiler Setup</h1>

          <div class="tabs">
            <button class="tab-button active" onclick=openTab('statusTab')>Status</button>
            <button class="tab-button" onclick=openTab('settingsTab')>Settings</button>
          </div>

          <div id="statusTab" class="tab-content active">
            <h2>Wi-Fi Status</h2>
            <div id="status">Status: Loading...</div>
          </div>

          <div id="settingsTab" class="tab-content">
            <div class="accordion">
              <div class="accordion-header" onclick=toggleAccordion(this)>Wi-Fi Settings</div>
              <div class="accordion-content">
                <button id="scanButton" onclick=scanNetworks() disabled>Scan Networks</button>
                <form id="wifiForm">
                  <label>Wi-Fi Network:</label>
                  <select name="ssid" id="ssidSelect"></select>
                  <label>Password:</label>
                  <input type="password" name="password" required>
                  <button type="submit">Connect</button>
                </form>
              </div>
            </div>

            <div class="accordion">
              <div class="accordion-header" onclick=toggleAccordion(this)>MQTT Settings</div>
              <div class="accordion-content">
                <form id="mqttForm">
                  <label>Server:</label>
                  <input type="text" name="host" value=")" + _configMgr.GetMQTTIP().toString() + R"(" required>
                  <label>Port:</label>
                  <input type="text" name="port" value=")" + String(_configMgr.GetMQTTPort()) + R"(" required>
                  <label>User:</label>
                  <input type="text" name="user" value=")" + String(_configMgr.GetMQTTUser().c_str()) + R"(">
                  <label>Password:</label>
                  <input type="password" name="password" value=")" + String(_configMgr.GetMQTTPass().c_str()) + R"(">
                  <label>Home Temperature Topic:</label>
                  <input type="text" name="homeTempTopic" value=")" + String(_configMgr.GetMQTTHomeTemperatureTopic().c_str()) + R"(" required>
                  <label>Outdoor Temperature Topic:</label>
                  <input type="text" name="outdoorTempTopic" value=")" + String(_configMgr.GetMQTTOutdoorTemperatureTopic().c_str()) + R"(" required>
                  <button type="submit">Save</button>
                </form>
              </div>
            </div>

            <div class="accordion">
              <div class="accordion-header" onclick=toggleAccordion(this)>Firmware Update</div>
              <div class="accordion-content">
                <form id="updateForm" enctype="multipart/form-data">
                  <label>Password:</label>
                  <input type="password" name="updatePassword" required>
                  <label>Firmware File:</label>
                  <input type="file" name="updateFile" accept=".bin" required>
                  <button type="submit">Update</button>
                </form>
              </div>
            </div>
          </div>
        </div>

        <div id="rebootOverlay">
          <div id="rebootIndicator">
            <div class="spinner"></div>
            <p id="rebootText">Rebooting...</p>
          </div>
        </div>

        <script>
          let scanInterval;

          function openTab(tabId) {
            const tabs = document.getElementsByClassName('tab-content');
            const buttons = document.getElementsByClassName('tab-button');
            for (let i = 0; i < tabs.length; i++) {
              tabs[i].classList.remove('active');
              buttons[i].classList.remove('active');
            }
            document.getElementById(tabId).classList.add('active');
            document.getElementsByClassName('tab-button')[tabId === 'statusTab' ? 0 : 1].classList.add('active');
          }

          function toggleAccordion(header) {
            const content = header.nextElementSibling;
            const allContents = document.getElementsByClassName('accordion-content');
            for (let i = 0; i < allContents.length; i++) {
              if (allContents[i] !== content) allContents[i].classList.remove('active');
            }
            content.classList.toggle('active');
          }

          function updateNetworks() {
            fetch('/networks')
              .then(response => response.json())
              .then(networks => {
                const select = document.getElementById('ssidSelect');
                const scanButton = document.getElementById('scanButton');
                select.innerHTML = '';
                if (networks.length === 0) {
                  select.disabled = true;
                  select.innerHTML = '<option selected>Поиск сетей...</option>';
                  scanButton.disabled = true;
                  if (!scanInterval) scanInterval = setInterval(updateNetworks, 5000);
                } else {
                  select.disabled = false;
                  scanButton.disabled = false;
                  networks.forEach(network => {
                    const option = document.createElement('option');
                    option.value = network.ssid;
                    option.text = `${network.ssid} (${network.rssi} dBm)`;
                    select.appendChild(option);
                  });
                  if (scanInterval) {
                    clearInterval(scanInterval);
                    scanInterval = null;
                  }
                }
              })
              .catch(() => {
                const select = document.getElementById('ssidSelect');
                const scanButton = document.getElementById('scanButton');
                select.disabled = true;
                select.innerHTML = '<option selected>Поиск сетей...</option>';
                scanButton.disabled = true;
                if (!scanInterval) scanInterval = setInterval(updateNetworks, 5000);
              });
          }

          function scanNetworks() {
            const select = document.getElementById('ssidSelect');
            const scanButton = document.getElementById('scanButton');
            select.innerHTML = '<option selected>Поиск сетей...</option>';
            select.disabled = true;
            scanButton.disabled = true;
            if (scanInterval) clearInterval(scanInterval);
            scanInterval = setInterval(updateNetworks, 5000);
            updateNetworks();
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
              if (text.includes('Rebooting')) {
                document.getElementById('rebootOverlay').style.display = 'block';
                document.getElementById('rebootText').innerHTML = 'Rebooting...';
                setTimeout(() => location.reload(), 3000);
              }
            })
            .catch(() => alert('Error connecting to Wi-Fi'));
          };

          document.getElementById('mqttForm').onsubmit = function(e) {
            e.preventDefault();
            fetch('/mqtt', { 
              method: 'POST', 
              body: new FormData(this) 
            })
            .then(response => response.text())
            .then(text => {
              if (text.includes('Rebooting')) {
                document.getElementById('rebootOverlay').style.display = 'block';
                document.getElementById('rebootText').innerHTML = 'Rebooting...';
                setTimeout(() => location.reload(), 3000);
              }
            })
            .catch(() => alert('Error saving MQTT settings'));
          };

          document.getElementById('updateForm').onsubmit = function(e) {
            e.preventDefault();
            const formData = new FormData(this);
            fetch('/update', {
              method: 'POST',
              body: formData
            })
            .then(response => response.text())
            .then(text => {
              if (text.includes('started')) {
                document.getElementById('rebootOverlay').style.display = 'block';
                document.getElementById('rebootText').innerHTML = 'Updating firmware...';
                setTimeout(() => location.reload(), 3000);
              }
            })
            .catch(() => alert('Error updating firmware'));
          };

          updateNetworks();
          updateStatus();
          setInterval(updateStatus, 5000);
        </script>
      </body>
      </html>
    )";
    request->send(200, "text/html", html); });
  _webserver->on("/mqtt", HTTP_POST, [this](AsyncWebServerRequest *request)
                 {
      if (!request->hasParam("host", true) || !request->hasParam("port", true) || 
          !request->hasParam("user", true) || !request->hasParam("password", true) ||
          !request->hasParam("homeTempTopic", true) || !request->hasParam("outdoorTempTopic", true)) {
        request->send(400, "text/plain", "Invalid request");
        return;
      }
      _configMgr.SetMQTTIP(request->getParam("host", true)->value().c_str());
      _configMgr.SetMQTTPort(request->getParam("port", true)->value().toInt());
      _configMgr.SetMQTTUser(request->getParam("user", true)->value().c_str());
      _configMgr.SetMQTTPass(request->getParam("password", true)->value().c_str());
      _configMgr.SetMQTTHomeTemperatureTopic(request->getParam("homeTempTopic", true)->value().c_str());
      _configMgr.SetMQTTOutdoorTemperatureTopic(request->getParam("outdoorTempTopic", true)->value().c_str());
      request->send(200, "text/plain", "MQTT settings saved. Rebooting...");
      _shouldReboot = true; });
  _webserver->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request)
                 {
                   if (!request->hasParam("updatePassword", true) || !request->hasParam("updateFile", true, true))
                   {
                     request->send(400, "text/plain", "Invalid request: Password or file missing");
                     return;
                   }

                   String password = request->getParam("updatePassword", true)->value();
                   if (password != FIRMWARE_UPDATE_PASSWORD)
                   {
                     request->send(401, "text/plain", "Invalid password");
                     return;
                   }

                   request->send(200, "text/plain", "Update started...");
                   _shouldReboot = true; // Устанавливаем флаг перезагрузки после успешного обновления
                 },
                 [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
                 {
      if (!index) {
        Serial.println("Firmware update started");
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
          Update.printError(Serial);
        }
      }
      if (Update.write(data, len) != len) {
        Update.printError(Serial);
      }
      if (final) {
        if (Update.end(true)) {
          Serial.println("Firmware update successful");
        } else {
          Update.printError(Serial);
        }
      } });
  _webserver->on("/networks", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
      Serial.println("Handling /networks request");
      if (!_wifiManager.isWifiReady()) {
        request->send(503, "text/plain", "WiFi not ready");
        Serial.println("WiFi not ready");
        return;
      }
      String json = "[";
      std::vector<NetworkInfo> networks = _wifiManager.ScanNetworks();
      for (size_t i = 0; i < networks.size(); i++) {
        if (i) json += ",";
        json += "{\"ssid\":\"" + String(networks[i].ssid.c_str()) + "\",\"rssi\":" + String(networks[i].rssi) + "}";
      }
      json += "]";
      request->send(200, "application/json", json);
      Serial.println("Sent /networks response"); });

  _webserver->on("/status", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
    const char *json = _statusProvider.GetStatusJson();
    request->send(200, "application/json", json); });
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
    request->send(200, "text/plain", "MQTT settings saved"); 
    _shouldReboot = true; });
}

void WebServerControl::Loop()
{
  if (_shouldReboot)
  {
    delay(2000);
    ESP.restart();
  }
  if (_wifiManager.isWifiReady() && _dnsState)
  {
    _dnsserver->processNextRequest();
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