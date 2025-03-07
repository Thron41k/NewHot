//Classes/Helpers/Observers/IConfigObserver.h
#ifndef ICONFIGOBSERVER_H
#define ICONFIGOBSERVER_H

#include <Classes/DeviceConfig/ParametreType.h>
class IConfigObserver
{
public:
  virtual void OnConfigChanged(ParametreType param) = 0;
  virtual ~IConfigObserver() = default;
};

#endif