//Classes/Helpers/Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <iomanip>

class Utils
{
public:
    Utils(){};
    ~Utils() = default;
    static std::string FloatToString(float f) { 
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << f;
        std::string str = stream.str();
        return str;
     }

     static std::string IntToString(int i) { 
        std::ostringstream stream;
        stream << i;
        std::string str = stream.str();
        return str;
     }
};

#endif