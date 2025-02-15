#include <iostream>
#include <sstream>
#include <iomanip>

class Utils
{
public:
    Utils(){};
    ~Utils() = default;
    static const char* FloatToString(float f) { 
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << f;
        std::string str = stream.str();
        return str.c_str();
     }

     static const char* IntToString(int i) { 
        std::ostringstream stream;
        stream << i;
        std::string str = stream.str();
        return str.c_str();
     }
};
