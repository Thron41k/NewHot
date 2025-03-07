//Classes/Helpers/Defines.h
#ifndef DEFINES_H
#define DEFINES_H


//Pins
#define STEPPER_IN1 19
#define STEPPER_IN2 18
#define STEPPER_IN3 5
#define STEPPER_IN4 17
#define STEPPER_LIMSW_X 3
#define TEMPERATURE_SENSOR 4
#define DISPLAY_I2C_SDA 25
#define DISPLAY_I2C_SCL 26
#define ENCODER_BUTTON 34
#define ENCODER_LEFT 35
#define ENCODER_RIGHT 36

//Constants
#define STEPS_PER_REVOLUTION 2038
#define MAX_POSITION 1000
#define CONFIG_SAVE_TIMEOUT 5000
#define DISPLAY_ROTATION_TIMEOUT 3000
#define ENCODER_LONG_PRESS_TIMEOUT 3
#define TEST_MODE 1


//WIFI
#define AP_WIFI_SSID "HotDevice"
#define AP_WIFI_PASSWORD "123456789101112"
#define MAX_WIFI_CONNECTION_ATTEMPTS 5
#define WIFI_RECONNECTION_TIMEOUT 1000*60*10

//OTA
#define FIRMWARE_UPDATE_PASSWORD "admin123"

//MQTT
#define MQTT_REPORT_INTERVAL 5000

#endif