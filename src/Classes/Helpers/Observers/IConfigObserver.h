#include <Classes/DeviceConfig/ParametreType.h>
class IConfigObserver {
    public:
      virtual void OnConfigChanged(ParametreType param) = 0;
      virtual ~IConfigObserver() = default;
    };