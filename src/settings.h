
#ifndef __PROJECTIONFX_SETTINGS_H__
#define __PROJECTIONFX_SETTINGS_H__

#if __has_include("settings_custom.h")
#include "settings_custom.h"
#else

#define MQTT_HOST "mqtt-hostname.example.copm"
#define MQTT_USER "<<YOUR_USERNAME>>"
#define MQTT_PASSWORD "<<YOUR_PASSWORD>>"

#define LED_PIN D6
#define NUM_LEDS 60

#define DEBUG_SERIAL true
#endif

#endif