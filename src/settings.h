
#ifndef __PROJECTIONFX_SETTINGS_H__
#define __PROJECTIONFX_SETTINGS_H__

#if __has_include("settings_custom.h")
#include "settings_custom.h"
#else

#define MQTT_HOST "mqtt.example.com"
#define MQTT_USER "user"
#define MQTT_PASSWORD "password"

#ifndef LED_PIN
    #define LED_PIN D4
#endif    
#define NUM_LEDS 60
#define LED_MAX_MILLIAMP 500


#define DEBUG_SERIAL true
#endif

#endif