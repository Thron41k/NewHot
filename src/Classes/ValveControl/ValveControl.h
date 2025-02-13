#include "ValveControl.h"

ValveControl::ValveControl()
  : _stepperControl(std::make_unique<StepperControl>()) {}

int ValveControl::GetPositionFromPercent(short percent) const {
  return _stepperControl->GetMaxPosition() * percent / 100;
}

void ValveControl::SetPercentage(short percent) {
  percent = std::clamp(percent, static_cast<short>(0), static_cast<short>(100));
  _stepperControl->GoToPosition(GetPositionFromPercent(percent));
}