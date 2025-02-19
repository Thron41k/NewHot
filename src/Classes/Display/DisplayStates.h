#ifndef DISPLAYSTATES_H
#define DISPLAYSTATES_H

#include "DisplayStateEnum.h"
#include <Classes/DeviceStates/DeviceStates.h>
#include "DisplayRenderer.h"

class DisplayStates: public IObserver, public IEncoderEventObserver
{
private:
    DisplayStateEnum _displayState = MainScreen;
    const DeviceStates *_deviceStates; 
    const EncoderControl *_enc; 
    std::unique_ptr<DisplayRenderer> _renderer;
    short _slide_pos = 0;
    uint8_t _last_slide_update = 0;
public:
    DisplayStates(const DeviceStates &deviceStates,const EncoderControl &encoder);
    ~DisplayStates();
    void Update(ParametreType param_type) override;
    void UpdateModeSpecificDisplay(ModeType mode);
    void Loop();
    void Event(EncoderEventEnum event) override;
};

#endif