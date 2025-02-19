#ifndef NETWORKINFO_H
#define NETWORKINFO_H

#include <string>
struct NetworkInfo {
    std::string ssid;
    int32_t rssi;
};

#endif