#ifndef IENCODEREVENTOBSERVER_H
#define IENCODEREVENTOBSERVER_H

#include <Classes/Input/EncoderEventEnum.h>

class IEncoderEventObserver {
public:
    virtual ~IEncoderEventObserver() = default;
    virtual void Event(EncoderEventEnum event) = 0;
};

#endif