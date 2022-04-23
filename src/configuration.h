#if !defined(_PROJECTIONFX_CONFIGURATION_H__)
#define _PROJECTIONFX_CONFIGURATION_H__

#include <FS.h>
#include "settings.h"
#include <ArduinoJson.h>
#include <WiFiManager.h>

#ifdef ESP32
#include <SPIFFS.h>
#endif

WiFiManager wifiManager;

inline void configCallback();

class Configuration
{
public:
    void setupWifiPortal()
    {

        this->setupSPIFF();
        // wifiManager.resetSettings();

        wifiManager.setDebugOutput(false);
        WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqttHost.c_str(), 40);
        wifiManager.addParameter(&custom_mqtt_server);

        WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqttUser.c_str(), 40);
        wifiManager.addParameter(&custom_mqtt_user);

        WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqttPassword.c_str(), 40);
        wifiManager.addParameter(&custom_mqtt_password);

        wifiManager.setSaveConfigCallback(configCallback);
        wifiManager.autoConnect("ProjektionFX");

        mqttHost = custom_mqtt_server.getValue();
        mqttUser = custom_mqtt_user.getValue();
        mqttPassword = custom_mqtt_password.getValue();

        if (mqttHost.equals("mqtt.example.com"))
        {
            Serial.println("Resetting WiFi Settings -> No MQTT Host set");
            wifiManager.resetSettings();
            ESP.restart();
        }

        if (shouldSave)
        {
            Serial.println("Saving Config");
            this->save();
        }

        Serial.println("Configuration completed!");
    }
    const char *getMQTTHost() { return mqttHost.c_str(); };
    const char *getMQTTUser() { return mqttUser.c_str(); };
    const char *getMQTTPassword() { return mqttPassword.c_str(); };
    void enableSave()
    {
        this->shouldSave = true;
    }

private:
    bool shouldSave = false;
    String mqttHost = MQTT_HOST;
    String mqttUser = MQTT_USER;
    String mqttPassword = MQTT_PASSWORD;

    inline void save()
    {
        Serial.println("saving config");
#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.createObject();
#endif
        json["mqtt_host"] = mqttHost;
        json["mqtt_user"] = mqttUser;
        json["mqtt_password"] = mqttPassword;

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
        serializeJson(json, Serial);
        serializeJson(json, configFile);
#else
        json.printTo(Serial);
        json.printTo(configFile);
#endif
        configFile.close();
        // end save
    }
    inline void setupSPIFF()
    {
        Serial.println("mounting FS...");

        if (SPIFFS.begin(true))
        {
            Serial.println("mounted file system");
            if (SPIFFS.exists("/config.json"))
            {
                // file exists, reading and loading
                Serial.println("reading config file");
                File configFile = SPIFFS.open("/config.json", "r");
                if (configFile)
                {
                    Serial.println("opened config file");
                    size_t size = configFile.size();
                    // Allocate a buffer to store contents of the file.
                    std::unique_ptr<char[]> buf(new char[size]);

                    configFile.readBytes(buf.get(), size);

#ifdef ARDUINOJSON_VERSION_MAJOR >= 6
                    DynamicJsonDocument json(1024);
                    auto deserializeError = deserializeJson(json, buf.get());
                    // serializeJson(json, Serial);
                    if (!deserializeError)
                    {
#else
                    DynamicJsonBuffer jsonBuffer;
                    JsonObject &json = jsonBuffer.parseObject(buf.get());
                    json.printTo(Serial);
                    if (json.success())
                    {
#endif
                        // Serial.println("\nparsed json");
                        String host = json["mqtt_host"];
                        String user = json["mqtt_user"];
                        String password = json["mqtt_password"];

                        mqttHost = host;
                        mqttUser = user;
                        mqttPassword = password;

                        Serial.printf("Config Restored\n");
                        Serial.printf(" mqtt_host:\t %s\n", mqttHost.c_str());
                        Serial.printf(" mqtt_user:\t %s\n", mqttUser.c_str());
                        Serial.printf(" mqtt_password:\t <HIDDEN>\n");
                    }
                    else
                    {
                        Serial.println("failed to load json config");
                    }
                    configFile.close();
                }
            }
        }
        else
        {
            Serial.println("failed to mount FS");
        }
    }
};

static Configuration config;

inline void configCallback()
{
    Serial.println("Config persist!");
    config.enableSave();
};

#endif // _PROJECTIONFX_CONFIGURATION_H__
