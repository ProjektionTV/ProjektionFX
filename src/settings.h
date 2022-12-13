#ifndef __PROJECTIONFX_SETTINGS_H__
#define __PROJECTIONFX_SETTINGS_H__

#if __has_include("settings_custom.h")
#include "settings_custom.h"
#else

#define MQTT_HOST "cb.twitchbridge.de"
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#ifndef LED_PIN
    #ifdef ESP32
    #define LED_PIN 5
    #else
    #define LED_PIN D4
    #endif
#endif
#define MAX_NUM_LEDS 1024
#define NUM_LEDS 144
#define LED_MAX_MILLIAMP 500

#define E131_ENABLED
#define UNIVERSE 1

#define DEBUG_SERIAL true
#endif

#endif