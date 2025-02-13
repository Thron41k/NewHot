#include <GyverDS18.h>
#include <memory>
#include <algorithm>
#include <Classes/Helpers/Logger.h>

class BoilerTemperature
{
private:
    Logger _logger;
    uint32_t _tmr;
    float _temp;
    std::unique_ptr<GyverDS18Single> _ds;

public:
    BoilerTemperature(Logger logger);
    ~BoilerTemperature() = default;
    void Loop();
    float GetTemp() const { return _temp; };
};