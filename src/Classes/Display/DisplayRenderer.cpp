#include "DisplayRenderer.h"

void DisplayRenderer::RenderMode(ModeType mode)
{
    _lcd->setCursor(0, 0);
    switch (mode)
    {
    case ModeType::Auto:
        _lcd->print("ABTO");
        _lcd->setCursor(0, 11);
        _lcd->write(126);
        break;
    case ModeType::Manual:
        _lcd->print("РУЧН");
        break;
    default:
        break;
    }
}

void DisplayRenderer::RenderModeAutoCurrentTemp(float temp)
{
    short pos = 6;
    _lcd->setCursor(0, pos);
    _lcd->print("     ");
    std::string result = Utils::FloatToString(temp);
    pos = pos - (5 - result.length());
    _lcd->print(result.c_str());
}

void DisplayRenderer::RenderModeAutoTargetTemp(float temp)
{
    _lcd->setCursor(0, 12);
    _lcd->print("     ");
    std::string result = Utils::FloatToString(temp);
    _lcd->setCursor(0, 12);
    _lcd->print(result.c_str());
}

void DisplayRenderer::RenderModeManualValvePercent(short percent)
{
    const short width = 11;
    _lcd->setCursor(0, 6);
    _lcd->print(std::string(width, ' ').c_str());
    std::string result = Utils::IntToString(percent) + "%";
    short startPos = (width - result.length()) / 2;
    _lcd->setCursor(0, startPos);
    _lcd->print(result.c_str());
}

void DisplayRenderer::RenderSlideInfo(std::string info)
{
    _lcd->setCursor(1, 0);
    _lcd->print(std::string(16, ' ').c_str());
    short startPos = (16 - info.length()) / 2;
    _lcd->setCursor(1, startPos);
    _lcd->print(info.c_str());
}

DisplayRenderer::DisplayRenderer()
{
    _lcd = std::make_unique<LiquidCrystal_I2C>(0x27, 16, 2);
    _lcd->init(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);
    _lcd->backlight();
}