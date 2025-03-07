//Classes/Helpers/Logger.h
#ifndef LOGGER_H
#define LOGGER_H

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

#endif