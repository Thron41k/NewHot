#include "EncoderControl.h"
#include "Classes/Helpers/Observers/IEncoderObserver.h"

void EncoderControl::NotifyObservers(EncoderEventEnum event)
{
    for (auto observer : observers)
    {
        observer->OnEncoderEvent(event);
    }
}

void EncoderControl::Attach(IEncoderObserver *observer)
{
    observers.push_back(observer);
}

void EncoderControl::Detach(IEncoderObserver *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void EncoderControl::Loop()
{
    _enc->tick();
    if(_enc->left()){
        NotifyObservers(Left);
    }
    if(_enc->right()){
        NotifyObservers(Right);
    }
    if(_enc->click()){
        NotifyObservers(Press);
    }
    if(_enc->hold(ENCODER_LONG_PRESS_TIMEOUT)){
        NotifyObservers(LongPress);
    }
    if(_enc->hasClicks(2)){
        NotifyObservers(DoublePress);
    }
}
