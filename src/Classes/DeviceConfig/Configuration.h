#include <Classes/DeviceStates/ModeType.h>

struct Configuration {
    int device_uid = 77234566436;
    ModeType mode = ModeType::Manual;
    float homeTargetTemp = 25.5;
    short valvePercent = 10;
    float boilerMaxTemp = 50.0;
  };