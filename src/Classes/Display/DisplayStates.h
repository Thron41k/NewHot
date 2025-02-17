#ifndef DISPLAYSTATES_H
#define DISPLAYSTATES_H

#include "DisplayStateEnum.h"
#include <Classes/DeviceStates/DeviceStates.h>
class DisplayStates: public IObserver 
{
private:
    DisplayStateEnum _displayState = MainScreen;
    const DeviceStates *_deviceStates; 
public:
    DisplayStates(const DeviceStates &_deviceStates);
    ~DisplayStates();
    void Update() override;
};

#endif