#include "DisplayRenderer.h"

void DisplayRenderer::PrintHomeTemp()
{
    if (!_deviceStates)
        return;
    _lcd->setCursor(0, 6);
    _lcd->print("           ");
    float currentTemp = _deviceStates->GetCurrentHomeTemp();
    float targetTemp = _deviceStates->GetDeviceConfig()->GetConfig().homeTargetTemp;
    uint8_t startPos = 6;
    if (currentTemp >= 10.0f)
        startPos += 1;
    if (currentTemp >= 0.0f && currentTemp < 10.0f)
        startPos += 2;
    _lcd->setCursor(0, startPos);
    _lcd->print(Utils::FloatToString(currentTemp));
    _lcd->write(126);
    _lcd->print(Utils::FloatToString(targetTemp));
}

void DisplayRenderer::PrintValvePercent()
{
    short percent = _deviceStates->GetValvePercent();
    int spaces = (percent < 10) ? 2 : (percent < 100) ? 1
                                                      : 0;
    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%*d%%", spaces + 1, percent);
    _lcd->setCursor(0, 8);
    _lcd->print(buffer);
}


DisplayRenderer::DisplayRenderer(const DeviceStates &_deviceStates)
    : _deviceStates(&_deviceStates)
{
    _lcd = std::make_unique<LiquidCrystal_I2C>(0x27, 16, 2);
    _lcd->init(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);
    _lcd->backlight();
}
 /*void DisplayRenderer::Update()
{
    if (!_deviceStates)
        return;

    _lcd->clear();
    _lcd->setCursor(0, 0);

    auto mode = _deviceStates->GetDeviceConfig()->GetConfig().mode;

    if (mode == ModeType::Auto)
    {
        _lcd->print("ABTO");
        PrintHomeTemp();
    }
    else if (mode == ModeType::Manual)
    {
        _lcd->print("РУЧН");
        PrintValvePercent();
    }
    float temp = _rotation_state ? _deviceStates->GetBoilerTemp() : _deviceStates->GetOutdoorTemp();
    _lcd->setCursor(1, 9);
    if (std::abs(temp) < 10)
        _lcd->print(" ");
    _lcd->print(Utils::FloatToString(temp));
    _lcd->print("°C");
}*/

void DisplayRenderer::Loop()
{
    if (!_deviceStates)
        return;
    if (millis() - _rotation_timeout > DISPLAY_ROTATION_TIMEOUT)
    {
        _rotation_timeout = millis();
        _lcd->setCursor(1, 3);
        float temp = _rotation_state ? _deviceStates->GetBoilerTemp() : _deviceStates->GetOutdoorTemp();
        _lcd->print(_rotation_state ? "КОТЁЛ " : "УЛИЦА ");
        if (std::abs(temp) < 10)
            _lcd->print(" ");
        _lcd->print(Utils::FloatToString(temp));
        _lcd->print("°C");
        _rotation_state = !_rotation_state;
    }
}
