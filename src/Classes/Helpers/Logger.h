#include <Arduino.h>

class Logger{
    public:
        void Log(String message){
            Serial.println(message);
        }

        void Error(String message){
            Serial.println(message);
        }
};