#include "WiFiControl.h"

WebServerControl::WebServerControl(WiFiControl &w) : _webserver(std::make_unique<AsyncWebServer>(80)), _dnsserver(std::make_unique<DNSServer>()), _wifiControl(w)
{
    StartDNS();
    _webserver->on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        std::string html = "<html><head><title>Настройка Wi-Fi</title></head><body>";
        html += "<h1>Выберите сеть Wi-Fi</h1>";
        html += "<form action='/connect' method='POST'>";
        html += "<select name='ssid'>";
        std::vector<NetworkInfo> networks = _wifiControl.ScanNetworks();
        if (!networks.empty())
        {
            for (const auto &network : networks)
            {
                html += "<option value='" + network.ssid + "'>" + network.ssid + "</option>";
            }
        }
        html += "</select>";
        html += "<br><br>Пароль: <input type='password' name='password'>";
        html += "<br><br><input type='submit' value='Подключиться'>";
        html += "</form>";
        html += "<br><br><h1>Настройка MQTT</h1>";
        html += "<form action='/mqtt' method='POST'>";
        html += "<br>Сервер: <input type='text' name='host' value='" + std::string(_wifiControl.GetDeviceStates()->GetMQTTIP().toString().c_str()) + "'>";
        html += "<br><br>Порт: <input type='text' name='port' value='" + std::to_string(_wifiControl.GetDeviceStates()->GetMQTTPort()) + "'>";
        html += "<br><br>Пользователь: <input type='text' name='user' value='" + std::string(_wifiControl.GetDeviceStates()->GetMQTTUser().c_str()) + "'>";
        html += "<br><br>Пароль: <input type='password' name='password' value='" + std::string(_wifiControl.GetDeviceStates()->GetMQTTPass().c_str()) + "'>";
        html += "<br><br><input type='submit' value='Сохранить'></form></body></html>";
        request->send(200, "text/html", html.c_str()); });
    _webserver->on("/connect", HTTP_POST, [this](AsyncWebServerRequest *request)
                   {
        if (!request->hasParam("ssid", true) || !request->hasParam("password", true)) {
            request->send(400, "text/plain", "Неверный запрос");
            return;
        }
        String ssid = request->getParam("ssid", true)->value();
        String password = request->getParam("password", true)->value();
        bool result = _wifiControl.ConnectToAP(ssid.c_str(),password.c_str());
        if (result) {
            request->send(200, "text/html", "<h1>Подключено к " + ssid + "</h1><p>Контроллер перезагрузится...</p>");
            shouldReboot = true;
        } else {
            request->send(200, "text/html", "<h1>Ошибка подключения</h1><p>Попробуйте снова.</p>");
        } });
    _webserver->on("/mqtt", HTTP_POST, [this](AsyncWebServerRequest *request)
                   {
        if (!request->hasParam("host", true) || !request->hasParam("port", true) || !request->hasParam("user", true) || !request->hasParam("password", true)) {
            return;
        }else{
            _wifiControl.GetDeviceStates()->SetMQTTIP(request->getParam("host",true)->value().c_str());
            _wifiControl.GetDeviceStates()->SetMQTTPort(request->getParam("port",true)->value().toInt());
            _wifiControl.GetDeviceStates()->SetMQTTUser(request->getParam("user",true)->value().c_str());
            _wifiControl.GetDeviceStates()->SetMQTTPass(request->getParam("password",true)->value().c_str());
        }
        });
    _webserver->begin();
}

void WebServerControl::Loop()
{
    if (shouldReboot)
    {
        delay(2000);
        ESP.restart();
    }
    _dnsserver->processNextRequest();
}

void WebServerControl::StartDNS()
{
    if (_dns_state)
        _dnsserver->stop();
    _dns_state = _dnsserver->start(53, "*", WiFi.softAPIP());
}

void WebServerControl::StopDNS()
{
    if (_dns_state)
        _dnsserver->stop();
    _dns_state = false;
}
