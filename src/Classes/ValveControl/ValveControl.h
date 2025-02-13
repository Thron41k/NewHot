#include "..\StepperControl\StepperControl.h"

class ValveControl {
private:
  std::unique_ptr<StepperControl> _stepperControl;
public:
  ValveControl();
  ~ValveControl() = default;
  void SetPercentage(short percent);
  short GetPercent();
};