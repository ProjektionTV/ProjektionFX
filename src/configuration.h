#ifndef _PROJECTIONFX_CONFIGURATION_H__
#define _PROJECTIONFX_CONFIGURATION_H__

#ifndef CONFIG_ESP32_PHY_MAX_TX_POWER
    #define CONFIG_ESP32_PHY_MAX_TX_POWER CONFIG_ESP_PHY_MAX_WIFI_TX_POWER
#endif


#include <FS.h>
#include "settings.h"
#include <ArduinoJson.h>
#include <WiFiManager.h>

#ifdef ESP32
#include <SPIFFS.h>
#endif


class Configuration
{
public:
    void setupWifiPortal(String hostName, bool configPortal);
    void connectionGuard();

    const char *getMQTTHost() { return mqttHost.c_str(); };
    const char *getMQTTUser() { return mqttUser.c_str(); };
    const char *getMQTTPassword() { return mqttPassword.c_str(); };
    uint16_t getNumLeds() {return numLeds.toInt();};
    uint16_t getMaxMilliamps() {return maxMilliamps.toInt();};
    uint16_t getUniverse() {return universe.toInt();};

private:
    String mqttHost = MQTT_HOST;
    String mqttUser = MQTT_USER;
    String mqttPassword = MQTT_PASSWORD;
    String numLeds = String(NUM_LEDS);
    String maxMilliamps = String(LED_MAX_MILLIAMP);
    String universe = String(UNIVERSE);

    bool isClock = false;
    bool isMaster = false;

    void save();
    void setupSPIFF();
    static void enableSave();
    static void saveParamsCallback();
    static void saveConfigCallback();
    static bool shouldSave;
};


#endif // _PROJECTIONFX_CONFIGURATION_H__
