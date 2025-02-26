#ifndef ENCODER_CONTROL_H
#define ENCODER_CONTROL_H

#include <EncButton.h>
#include <memory>
#include "EncoderEventEnum.h"
#include "../Helpers/Defines.h"
#include <Classes/Helpers/Observers/IEncoderObserver.h>

class EncoderControl
{
private:
    std::unique_ptr<EncButton> _enc;
    std::vector<IEncoderObserver *> observers;
    void NotifyObservers(EncoderEventEnum event);

public:
    EncoderControl() : _enc(std::make_unique<EncButton>(ENCODER_LEFT, ENCODER_RIGHT, ENCODER_BUTTON)) {};
    ~EncoderControl() = default;
    void Attach(IEncoderObserver *observer);
    void Detach(IEncoderObserver *observer);
    void Loop();
};

#endif