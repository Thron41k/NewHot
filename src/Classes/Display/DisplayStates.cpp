#include "DisplayStates.h"

DisplayStates::DisplayStates(const DeviceStates &deviceStates, const EncoderControl &encoder) : _enc(&encoder), _deviceStates(&deviceStates), _renderer(std::make_unique<DisplayRenderer>())
{
    const_cast<DeviceStates &>(*_deviceStates).Attach(this);
    const_cast<EncoderControl &>(*_enc).Attach(this);
}

DisplayStates::~DisplayStates()
{
    if (_deviceStates)
    {
        const_cast<DeviceStates &>(*_deviceStates).Detach(this);
    }
    if (_enc)
    {
        const_cast<EncoderControl &>(*_enc).Detach(this);
    }
}

void DisplayStates::Update(ParametreType param_type)
{
    if (_displayState == MainScreen)
    {
        ModeType mode = _deviceStates->GetMode();

        switch (param_type)
        {
        case ParametreType::Mode:
            _renderer->RenderMode(mode);
            UpdateModeSpecificDisplay(mode);
            break;

        case ParametreType::HomeTargetTemp:
            if (mode == ModeType::Auto)
            {
                _renderer->RenderModeAutoTargetTemp(_deviceStates->GetTargetHomeTemp());
            }
            break;

        case ParametreType::HomeCurrentTemp:
            if (mode == ModeType::Auto)
            {
                _renderer->RenderModeAutoCurrentTemp(_deviceStates->GetCurrentHomeTemp());
            }
            break;

        case ParametreType::ValvePercent:
            if (mode == ModeType::Manual)
            {
                _renderer->RenderModeManualValvePercent(_deviceStates->GetValvePercent());
            }
            break;

        default:
            break;
        }
    }
}

void DisplayStates::UpdateModeSpecificDisplay(ModeType mode)
{
    if (_displayState == MainScreen)
    {
        switch (mode)
        {
        case ModeType::Auto:
            _renderer->RenderModeAutoCurrentTemp(_deviceStates->GetCurrentHomeTemp());
            _renderer->RenderModeAutoTargetTemp(_deviceStates->GetTargetHomeTemp());
            break;

        case ModeType::Manual:
            _renderer->RenderModeManualValvePercent(_deviceStates->GetValvePercent());
            break;

        default:
            break;
        }
    }
}

void DisplayStates::Loop()
{
    if (_displayState == MainScreen)
    {
        if (millis() - _last_slide_update >= DISPLAY_ROTATION_TIMEOUT)
        {
            _last_slide_update = millis();
            switch (_slide_pos)
            {
            case 0:
                _renderer->RenderSlideInfo("КОТЁЛ " + Utils::FloatToString(_deviceStates->GetBoilerTemp()) + "°C");
                _slide_pos = 1;
                break;
            case 1:
                _renderer->RenderSlideInfo("УЛИЦА " + Utils::FloatToString(_deviceStates->GetOutdoorTemp()) + "°C");
                _slide_pos = 2;
                break;
            case 2:
                _renderer->RenderSlideInfo("ДОМ " + Utils::FloatToString(_deviceStates->GetCurrentHomeTemp()) + "°C");
                _slide_pos = 0;
                break;
            default:
                break;
            }
        }
    }
}

void DisplayStates::Event(EncoderEventEnum event)
{
    if (event == EncoderEventEnum::Left)
    {
        if (_displayState == MainScreen)
        {
            ModeType mode = _deviceStates->GetMode();
            if (mode == ModeType::Auto)
            {
                _deviceStates->SetTargetHomeTemp(_deviceStates->GetTargetHomeTemp() - 1);
            }
            else if (mode == ModeType::Manual)
            {
                _deviceStates->SetValvePercent(_deviceStates->GetValvePercent() - 1);
            }
        }
    }
    if (event == EncoderEventEnum::Right)
    {
        if (_displayState == MainScreen)
        {
            ModeType mode = _deviceStates->GetMode();
            if (mode == ModeType::Auto)
            {
                _deviceStates->SetTargetHomeTemp(_deviceStates->GetTargetHomeTemp() + 1);
            }
            else if (mode == ModeType::Manual)
            {
                _deviceStates->SetValvePercent(_deviceStates->GetValvePercent() + 1);
            }
        }
    }
}
