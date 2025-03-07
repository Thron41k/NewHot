//Classes/WiFiControl/Interfaces/IWebServer.h
#ifndef IWEBSERVER_H
#define IWEBSERVER_H

class IWebServer {
public:
  virtual void StartDNS() = 0;
  virtual void StopDNS() = 0;
  virtual void Loop() = 0;
  virtual ~IWebServer() = default;
};

#endif