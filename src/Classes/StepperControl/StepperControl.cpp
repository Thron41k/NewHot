//Classes/StepperControl/StepperControl.cpp
#include "StepperControl.h"

StepperControl::StepperControl()
{
  Serial.println("StepperControl initialized 0");
  pinMode(STEPPER_LIMSW_X, INPUT_PULLUP);
  Serial.println("StepperControl initialized 1");
  _stepper = std::make_unique<GStepper2<STEPPER4WIRE>>(STEPS_PER_REVOLUTION, STEPPER_IN1, STEPPER_IN2, STEPPER_IN3, STEPPER_IN4);
  Serial.println("StepperControl initialized 2");
  //Homing();
  Serial.println("StepperControl initialized 3");
}

void StepperControl::GoToPositionByPercent(short percent)
{
  int position = MAX_POSITION * percent / 100;
  if (!inHome || position < 0 || position > MAX_POSITION)
  {
    return;
  }
  _stepper->setMaxSpeed(100);
  _stepper->setAcceleration(200);
  _stepper->setTarget(position);
  xTaskCreatePinnedToCore(StartGoToPositionTaskImpl, "GoToPosition", 10000, this, 1, NULL, 1);
}

void StepperControl::Homing()
{
  xTaskCreatePinnedToCore(StartHomingTaskImpl, "Homing", 10000, this, 1, NULL, 1);
}

void StepperControl::StartGoToPositionTask()
{
  while (!_stepper->ready())
  {
    _stepper->tick();
  }
}

void StepperControl::StartHomingTask()
{
  if (digitalRead(STEPPER_LIMSW_X))
  {
    _stepper->setSpeed(-10.0);
    while (digitalRead(STEPPER_LIMSW_X))
    {
      _stepper->tick();
    }
    _stepper->brake();
  }
  inHome = true;
  _stepper->reset();
}

short StepperControl::GetCurrentPercent()
{
  return 100 * _stepper->pos / MAX_POSITION;
}