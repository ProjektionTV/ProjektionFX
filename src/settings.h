
#ifndef __PROJECTIONFX_SETTINGS_H__
#define __PROJECTIONFX_SETTINGS_H__

#if __has_include("settings_custom.h")
#include "settings_custom.h"
#else

#define MQTT_HOST "mqtt.example.com"
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#ifndef LED_PIN
    #define LED_PIN D6
#endif    
#define NUM_LEDS 256

#define DEBUG_SERIAL true
#endif

#endif