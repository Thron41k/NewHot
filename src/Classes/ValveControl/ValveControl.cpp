#include "ValveControl.h"

ValveControl::ValveControl()
    : _stepperControl(std::make_unique<StepperControl>()) {}

void ValveControl::SetPercentage(short percent)
{
  percent = std::clamp(percent, static_cast<short>(0), static_cast<short>(100));
  _stepperControl->GoToPositionByPercent(percent);
}

short ValveControl::GetPercent()
{
  return _stepperControl->GetCurrentPercent();
}
