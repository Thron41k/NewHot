#include <GyverStepper2.h>
#include <memory>
#include <algorithm>

#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17
#define LIMSW_X 6

const int stepsPerRevolution = 2038;
const int maxPosition = 1000;

class StepperControl
{
private:
  std::unique_ptr<GStepper2<STEPPER4WIRE>> _stepper;
  bool inHome = false;

  void StartGoToPositionTask();
  void StartHomingTask();
  static void StartGoToPositionTaskImpl(void *_this)
  {
    static_cast<StepperControl *>(_this)->StartGoToPositionTask();
  }

  static void StartHomingTaskImpl(void *_this)
  {
    static_cast<StepperControl *>(_this)->StartHomingTask();
  }

public:
  StepperControl();
  ~StepperControl() = default;
  void GoToPosition(int position);
  void Homing();
  int GetMaxPosition() const
  {
    return maxPosition;
  }
};
