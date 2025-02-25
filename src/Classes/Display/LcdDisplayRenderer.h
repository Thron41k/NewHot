#include "Classes/Display/Interfaces/IDisplayRenderer.h"
#include "memory"
#include <LiquidCrystal_I2C.h>
#include <Classes/Helpers/Defines.h>
#include <Classes/Helpers/Utils.h>

class LcdDisplayRenderer : public IDisplayRenderer {
private:
  std::unique_ptr<LiquidCrystal_I2C> _lcd;
public:
  LcdDisplayRenderer() {
    _lcd = std::make_unique<LiquidCrystal_I2C>(0x27, 16, 2);
    _lcd->init(DISPLAY_I2C_SDA, DISPLAY_I2C_SCL);
    _lcd->backlight();
  }

  void RenderMode(ModeType mode) override {
    _lcd->setCursor(0, 0);
    switch (mode) {
      case ModeType::Auto:
        _lcd->print("ABTO");
        _lcd->setCursor(0, 11);
        _lcd->write(126);
        break;
      case ModeType::Manual:
        _lcd->print("РУЧН");
        break;
    }
  }

  void RenderBoilerTemp(float temp) override {
    RenderSlideInfo("КОТЁЛ " + String(Utils::FloatToString(temp).c_str()) + "°C");
  }

  void RenderHomeTemp(float temp) override {
    short pos = 6;
    _lcd->setCursor(0, pos);
    _lcd->print("     ");
    String result = Utils::FloatToString(temp).c_str();
    pos = pos - (5 - result.length());
    _lcd->print(result);
  }

  void RenderOutdoorTemp(float temp) override {
    RenderSlideInfo("УЛИЦА " + String(Utils::FloatToString(temp).c_str()) + "°C");
  }

  void RenderTargetHomeTemp(float temp) override {
    _lcd->setCursor(0, 12);
    _lcd->print("     ");
    String result = Utils::FloatToString(temp).c_str();
    _lcd->setCursor(0, 12);
    _lcd->print(result);
  }

  void RenderValvePercent(short percent) override {
    const short width = 11;
    _lcd->setCursor(0, 6);
    _lcd->print(String(width, ' '));
    String result = String(Utils::IntToString(percent).c_str()) + "%";
    short startPos = (width - result.length()) / 2;
    _lcd->setCursor(0, startPos);
    _lcd->print(result);
  }

  void RenderSlideInfo(const String& info) override {
    _lcd->setCursor(1, 0);
    _lcd->print(String(16, ' '));
    short startPos = (16 - info.length()) / 2;
    _lcd->setCursor(1, startPos);
    _lcd->print(info);
  }
};