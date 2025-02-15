#include "ValveControl.h"

ValveControl::ValveControl()
    : _stepperControl(std::make_unique<StepperControl>()) {}

void ValveControl::SetPercentage(short percent)
{
  _stepperControl->GoToPositionByPercent(percent);
}

short ValveControl::GetPercent()
{
  return _stepperControl->GetCurrentPercent();
}
