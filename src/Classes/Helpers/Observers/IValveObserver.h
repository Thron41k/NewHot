//Classes/Helpers/Observers/IValveObserver.h
#ifndef IVALVEOBSERVER_H
#define IVALVEOBSERVER_H

class IValveObserver
{
public:
  virtual void OnValvePercentChanged(short percent) = 0;
  virtual ~IValveObserver() = default;
};

#endif