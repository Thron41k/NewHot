#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <Classes/DeviceConfig/ParametreType.h>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update(ParametreType param_type) = 0;
};

#endif