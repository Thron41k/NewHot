#ifndef ISTATUSPROVIDER_H
#define ISTATUSPROVIDER_H

class IStatusProvider {
  public:
    virtual const char* GetStatusJson() const = 0;
    virtual ~IStatusProvider() = default;
  };
#endif