#ifndef __PROJECTIONFX_SETTINGS_H__
#define __PROJECTIONFX_SETTINGS_H__

#if __has_include("settings_custom.h")
#include "settings_custom.h"
#else

#define MQTT_HOST "mqtt.example.com"
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#ifndef LED_PIN
    #ifdef ESP32
    #define LED_PIN 4
    #else
    #define LED_PIN D4
    #endif
#endif
#define NUM_LEDS 60
#define LED_MAX_MILLIAMP 500

#define E131_ENABLED 1

#define DEBUG_SERIAL true
#endif

#endif