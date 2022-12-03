#include <configuration.h>

WiFiManager wifiManager;

Configuration config;

bool Configuration::shouldSave = false;

void Configuration::enableSave()
{
    shouldSave = true;
}

void Configuration::setupWifiPortal(String hostName, bool configPortal)
{

    setupSPIFF();

    WiFi.setHostname(hostName.c_str());
    wifiManager.setHostname(hostName.c_str());
    // wifiManager.resetSettings();

    wifiManager.setDarkMode(true);


    wifiManager.setDebugOutput(false);
    wifiManager.setConfigPortalTimeout(300);

    // WiFiManagerParameter custom_html("<p>This Is Custom HTML</p>"); // only custom html
    // wifiManager.addParameter(&custom_html);

    WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqttHost.c_str(), 40);
    wifiManager.addParameter(&custom_mqtt_server);

    WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqttUser.c_str(), 40);
    wifiManager.addParameter(&custom_mqtt_user);

    WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqttPassword.c_str(), 40);
    wifiManager.addParameter(&custom_mqtt_password);

    WiFiManagerParameter custom_num_leds("num_leds", "Number of LEDs", numLeds.c_str(), 5);
    wifiManager.addParameter(&custom_num_leds);

    WiFiManagerParameter custom_max_milliamps("max_milliamps", "max Milliamps", maxMilliamps.c_str(), 5);
    wifiManager.addParameter(&custom_max_milliamps);

    char _custom_checkbox_clock[32] = "type=\"checkbox\"";

    if(isClock)
    {
        strcat(_custom_checkbox_clock, " checked");
    }

    WiFiManagerParameter custom_checkbox_is_clock("isclock", "Clock", "T", 2, _custom_checkbox_clock, WFM_LABEL_AFTER);
    wifiManager.addParameter(&custom_checkbox_is_clock);

    char _custom_checkbox_master[32] = "type=\"checkbox\"";

    if(isMaster)
    {
        strcat(_custom_checkbox_master, " checked");
    }

    WiFiManagerParameter custom_checkbox_is_master("ismaster", "Master", "T", 2, _custom_checkbox_master, WFM_LABEL_AFTER);
    wifiManager.addParameter(&custom_checkbox_is_master);

    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.setSaveParamsCallback(saveParamsCallback);

    if (configPortal)
    {
        wifiManager.setConfigPortalBlocking(true);
        Serial.print("Starting ConfigPortal...");
        wifiManager.startConfigPortal(hostName.c_str());
    }
    else
    {
        Serial.print("Attempting WiFi connection... ");
        bool res = wifiManager.autoConnect(hostName.c_str(), NULL);
        if (!res)
        {
            Serial.println("failed! -> Reset");
            delay(2500);
            ESP.restart();
        }
        else
        {
            Serial.printf("connected, IP: %s\n", WiFi.localIP().toString().c_str());
        }             
    }
    mqttHost = custom_mqtt_server.getValue();
    mqttUser = custom_mqtt_user.getValue();
    mqttPassword = custom_mqtt_password.getValue();
    numLeds = custom_num_leds.getValue();
    maxMilliamps = custom_max_milliamps.getValue();
    isClock = (strncmp(custom_checkbox_is_clock.getValue(), "T", 1) == 0);
    isMaster = (strncmp(custom_checkbox_is_master.getValue(), "T", 1) == 0);

    if (mqttHost.equals("mqtt.example.com"))
    {
        Serial.println("Resetting WiFi Settings -> No MQTT Host set");
        wifiManager.resetSettings();
        ESP.restart();
    }

    if (shouldSave)
    {
        Serial.println("shouldSave == true");
        save();
    }

    Serial.println("Configuration completed!");
}


void Configuration::connectionGuard()
{
    if(!WiFi.isConnected())
    {
        Serial.print("\nAttempting WiFi reconnect");
        WiFi.begin();
        static uint8_t tries = 120;

        while(!WiFi.isConnected())
        {
            if(!tries--)
            {
                Serial.println("\nfailed! -> Reset");
                delay(2500);
                ESP.restart();
            }
            Serial.print('.');
            delay(1000);
        }
        //server.begin();
        Serial.println(" connected.");
        Serial.println(WiFi.localIP());
    }       
}

void Configuration::save()
{
    Serial.println("saving config");
#if ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
#endif
    json["mqtt_host"] = mqttHost;
    json["mqtt_user"] = mqttUser;
    json["mqtt_password"] = mqttPassword;
    json["num_leds"] = numLeds;
    json["max_milliamps"] = maxMilliamps;
    json["is_clock"] = isClock;
    json["is_master"] = isMaster;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
        Serial.println("failed to open config file for writing");
    }

#if ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    // end save
}


void Configuration::setupSPIFF()
{
    Serial.println("mounting FS...");

#ifdef ESP32
    if (SPIFFS.begin(true))
#else
    if (SPIFFS.begin())
#endif
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

#if ARDUINOJSON_VERSION_MAJOR >= 6
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
                    String numleds = json["num_leds"];
                    String maxmilliamps = json["max_milliamps"];
                    bool isclock = json["is_clock"];
                    bool ismaster = json["is_master"];

                    mqttHost = host;
                    mqttUser = user;
                    mqttPassword = password;
                    numLeds = numleds;
                    maxMilliamps = maxmilliamps;
                    isClock = isclock;
                    isMaster = ismaster;

                    Serial.printf("Config Restored\n");
                    Serial.printf(" mqtt_host:\t %s\n", mqttHost.c_str());
                    Serial.printf(" mqtt_user:\t %s\n", mqttUser.c_str());
                    Serial.printf(" mqtt_password:\t <HIDDEN>\n");
                    Serial.printf(" num_leds:\t %s\n", numLeds.c_str());
                    Serial.printf(" max_milliamps:\t %s\n", maxMilliamps.c_str());
                    Serial.printf(" is_clock:\t %d\n", isClock);
                    Serial.printf(" is_master:\t %d\n", isMaster);
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


void Configuration::saveParamsCallback()
{
    Serial.println("saveParamsCallback");
    enableSave();
    wifiManager.stopConfigPortal();
};

void Configuration::saveConfigCallback()
{
    Serial.println("saveConfigCallback");
    enableSave();
};