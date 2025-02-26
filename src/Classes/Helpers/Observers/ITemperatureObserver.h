#ifndef ITEMPERATUREOBSERVER_H
#define ITEMPERATUREOBSERVER_H

class ITemperatureObserver {
    public:
      virtual void OnBoilerTempChanged(float temp) = 0;
      virtual ~ITemperatureObserver() = default;
    };

#endif