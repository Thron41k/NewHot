#include "DisplayController.h"

void DisplayController::UpdateSlideShow() {
    if (millis() - _lastSlideUpdate >= DISPLAY_ROTATION_TIMEOUT) {
      _lastSlideUpdate = millis();
      switch (_slidePos) {
        case 0:
          _renderer->RenderBoilerTemp(_tempMgr.GetBoilerTemp());
          _slidePos = 1;
          break;
        case 1:
          _renderer->RenderOutdoorTemp(_tempMgr.GetOutdoorTemp());
          _slidePos = 2;
          break;
        case 2:
          _renderer->RenderHomeTemp(_tempMgr.GetHomeTemp());
          _slidePos = 0;
          break;
      }
    }
  }