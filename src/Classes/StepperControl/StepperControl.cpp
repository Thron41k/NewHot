#include "StepperControl.h"

StepperControl::StepperControl() {
  pinMode(LIMSW_X, INPUT_PULLUP);
  _stepper = std::make_unique<GStepper2<STEPPER4WIRE>>(stepsPerRevolution, IN1, IN2, IN3, IN4);
  Homing();
}

void StepperControl::GoToPosition(int position) {
  if (!inHome || position < 0 || position > maxPosition) {
    return;
  }
  _stepper->setMaxSpeed(100);
  _stepper->setAcceleration(200);
  _stepper->setTarget(position);
  xTaskCreatePinnedToCore(StartGoToPositionTaskImpl, "GoToPosition", 10000, this, 1, NULL, 1);
}

void StepperControl::Homing() {
  xTaskCreatePinnedToCore(StartHomingTaskImpl, "Homing", 10000, this, 1, NULL, 1);
}

void StepperControl::StartGoToPositionTask() {
  while (!_stepper->ready()) {
    _stepper->tick();
  }
}

void StepperControl::StartHomingTask() {
  if (digitalRead(LIMSW_X)) {
    _stepper->setSpeed(-10.0);
    while (digitalRead(LIMSW_X)) {
      _stepper->tick();
    }
    _stepper->brake();
  }
  inHome = true;
  _stepper->reset();
}
