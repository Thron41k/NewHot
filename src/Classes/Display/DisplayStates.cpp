#include "DisplayStates.h"

DisplayStates::DisplayStates(const DeviceStates &_deviceStates) : _deviceStates(&_deviceStates)
{
    const_cast<DeviceStates&>(_deviceStates).Attach(this);
}

DisplayStates::~DisplayStates()
{
    if (_deviceStates) {
        const_cast<DeviceStates&>(*_deviceStates).Detach(this);
    }
}

void DisplayStates::Update()
{
    
}
