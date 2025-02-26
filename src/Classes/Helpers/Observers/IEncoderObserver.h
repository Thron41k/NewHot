#ifndef IENCODEROBSERVER_H
#define IENCODEROBSERVER_H

#include <Classes/Input/EncoderEventEnum.h>
class IEncoderObserver {
    public:
      virtual void OnEncoderEvent(EncoderEventEnum event) = 0;
      virtual ~IEncoderObserver() = default;
    };

#endif