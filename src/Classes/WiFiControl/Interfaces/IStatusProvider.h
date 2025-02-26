#ifndef ISTATUSPROVIDER_H
#define ISTATUSPROVIDER_H

#include <WString.h>

class IStatusProvider {
public:
  virtual String GetStatusJson() const = 0;
  virtual ~IStatusProvider() = default;
};

#endif