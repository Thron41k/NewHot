#include <GyverDS18.h>
#include <memory>
#include <algorithm>
#include <Classes/Helpers/Logger.h>
#include <Classes/Helpers/Defines.h>
#include <Classes/Helpers/Observer/IObserver.h>

class BoilerTemperature
{
private:
    Logger _logger;
    uint32_t _tmr;
    float _temp;
    std::unique_ptr<GyverDS18Single> _ds;
    std::vector<IObserver*> observers;
    void NotifyObservers();

public:
    BoilerTemperature(Logger logger);
    ~BoilerTemperature() = default;
    void Loop();
    float GetTemp() const { return _temp; };
    void Attach(IObserver* observer);
    void Detach(IObserver* observer);
};