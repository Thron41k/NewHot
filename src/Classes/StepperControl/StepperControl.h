//Classes/StepperControl/StepperControl.h
#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

#include <GyverStepper2.h>
#include <memory>
#include <algorithm>
#include "../Helpers/Defines.h"


class StepperControl {
private:
  std::unique_ptr<GStepper2<STEPPER4WIRE>> _stepper;
  bool inHome = false;

  void StartGoToPositionTask();
  void StartHomingTask();
  static void StartGoToPositionTaskImpl(void* _this) {
    static_cast<StepperControl*>(_this)->StartGoToPositionTask();
  }

  static void StartHomingTaskImpl(void* _this) {
    static_cast<StepperControl*>(_this)->StartHomingTask();
  }
public:
  StepperControl();
  ~StepperControl() = default;
  void GoToPositionByPercent(short percent);
  void Homing();
  short GetCurrentPercent();
  int GetMaxPosition() const {
    return MAX_POSITION;
  }
};

#endif