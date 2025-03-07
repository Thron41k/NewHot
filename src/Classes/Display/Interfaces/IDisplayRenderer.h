// Classes/Display/Interfaces/IDisplayRenderer.h
#ifndef DISPLAY_RENDERER_H
#define DISPLAY_RENDERER_H

#include <Classes/DeviceStates/ModeType.h>
#include <WString.h>

class IDisplayRenderer
{
public:
  virtual void RenderMode(ModeType mode) = 0;
  virtual void RenderBoilerTemp(float temp) = 0;
  virtual void RenderHomeTemp(float temp) = 0;
  virtual void RenderOutdoorTemp(float temp) = 0;
  virtual void RenderTargetHomeTemp(float temp) = 0;
  virtual void RenderValvePercent(short percent) = 0;
  virtual void RenderSlideInfo(const String &info) = 0;
  virtual ~IDisplayRenderer() = default;
};

#endif