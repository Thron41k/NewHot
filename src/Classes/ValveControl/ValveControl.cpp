#include "ValveControl.h"

ValveControl::ValveControl()
{
  Serial.println("ValveControl initialized 0");
  _stepperControl = std::make_unique<StepperControl>();
  Serial.println("ValveControl initialized 1");
}

void ValveControl::SetPercentage(short percent)
{
  _stepperControl->GoToPositionByPercent(percent);
}

short ValveControl::GetPercent()
{
  return _stepperControl->GetCurrentPercent();
}
