#include "..\StepperControl\StepperControl.h"

class ValveControl {
private:
  std::unique_ptr<StepperControl> _stepperControl;
  int GetPositionFromPercent(short percent) const;
public:
  ValveControl();
  ~ValveControl() = default;
  void SetPercentage(short percent);
};