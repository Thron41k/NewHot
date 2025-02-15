#include "Display.h"

Display::Display()
{
    _lcd = std::make_unique<LiquidCrystal_I2C>(0x27, 16, 2);
    _lcd->init(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);
	_lcd->backlight();
}

void Display::UpdateMode(ModeType mode)
{
    _lcd->setCursor(0,0);
    switch (mode)
    {
    case ModeType::Manual:
        _lcd->print("АВТО");
        break;
    case ModeType::Auto:
        _lcd->print("РУЧН");
    default:
        break;
    }
}
