#include <EncButton.h>
#include <memory>
#include "EncoderEventEnum.h"
#include <Classes/Helpers/Observer/IEncoderEventObserver.h>
#include "../Helpers/Defines.h"

class EncoderControl
{
private:
    std::unique_ptr<EncButton> _enc;
    std::vector<IEncoderEventObserver *> observers;
    void NotifyObservers(EncoderEventEnum event);

public:
    EncoderControl() : _enc(std::make_unique<EncButton>(ENCODER_LEFT, ENCODER_RIGHT, ENCODER_BUTTON)) {};
    ~EncoderControl() = default;
    void Attach(IEncoderEventObserver *observer);
    void Detach(IEncoderEventObserver *observer);
    void Loop();
};