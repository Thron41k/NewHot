#ifndef IOBSERVER_H
#define IOBSERVER_H

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update() = 0;
};

#endif