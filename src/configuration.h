#if !defined(_PROJECTIONFX_CONFIGURATION_H__)
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

// Set web server port number to 80
WiFiServer webServer(80);
// Variable to store the HTTP request
String header;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

WiFiManager wifiManager;

inline void configCallback();

class Configuration
{
public:
    void setupWifiPortal(String hostName)
    {

        this->setupSPIFF();

        WiFi.setHostname(hostName.c_str());
        // wifiManager.resetSettings();

        wifiManager.setDebugOutput(false);
        wifiManager.setConfigPortalTimeout(300);

        WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqttHost.c_str(), 40);
        wifiManager.addParameter(&custom_mqtt_server);

        WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqttUser.c_str(), 40);
        wifiManager.addParameter(&custom_mqtt_user);

        WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqttPassword.c_str(), 40);
        wifiManager.addParameter(&custom_mqtt_password);

        wifiManager.setSaveConfigCallback(configCallback);

        Serial.print("Attempting WiFi connection... ");
        
        wifiManager.setHostname(hostName.c_str());
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

    void connectionGuard()
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

    void webplayer()
    {
        WiFiClient webClient = webServer.available();   // Listen for incoming clients

        if (webClient) {                             // If a new client connects,
            currentTime = millis();
            previousTime = currentTime;
            Serial.println("New Client.");          // print a message out in the serial port
            String currentLine = "";                // make a String to hold incoming data from the client
            while (webClient.connected() && currentTime - previousTime <= timeoutTime)  // loop while the client's connected
            {
                currentTime = millis();
                // if there's bytes to read from the client,
                if (webClient.available())
                {      
                    char c = webClient.read();             // read a byte, then
                    Serial.write(c);                    // print it out the serial monitor
                    header += c;
                    if (c == '\n')                    // if the byte is a newline character
                    {
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0)
                        {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            webClient.println("HTTP/1.1 200 OK");
                            webClient.println("Content-type:text/html");
                            webClient.println("Connection: close");
                            webClient.println();

                            webClient.println("<!DOCTYPE html><html>");
                            webClient.println("<head>");
                            webClient.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                            webClient.println("<script src=\"https://player.twitch.tv/js/embed/v1.js\"></script>");
                            webClient.println("</head>");
                            webClient.println("<body>");
                            webClient.println("<div id=\"twitchplayer\"></div>");
                            webClient.println("<br>");
                            webClient.println("<div id=\"delay\"></div>");
                            webClient.println("<script type=\"text/javascript\">");
                            webClient.println("    //countdown function is evoked when page is loaded");
                            webClient.println("    function countdown() {");
                            webClient.println("        document.getElementById(\"delay\").innerHTML = player.getPlaybackStats().hlsLatencyBroadcaster;");
                            webClient.println("    }");
                            webClient.println("    var options = {");
                            webClient.println("        width: 1280,");
                            webClient.println("        height: 720,");
                            webClient.println("        channel: \"projektiontv\",");
                            webClient.println("        parent: ['esp32-192.168.0.101.local']");
                            webClient.println("    };");
                            webClient.println("    var player = new Twitch.Player(\"twitchplayer\", options);");
                            webClient.println("    player.setVolume(0.5);");
                            webClient.println("    setInterval(countdown, 5000);");
                            webClient.println("</script>");
                            webClient.println("</body>");
                            webClient.println("</html>");

                            // The HTTP response ends with another blank line
                            webClient.println();
                            // Break out of the while loop
                            break;
                        }
                        else { // if you got a newline, then clear currentLine
                            currentLine = "";
                        }
                    }
                    else if (c != '\r') {  // if you got anything else but a carriage return character,
                        currentLine += c;      // add it to the end of the currentLine
                    }
                }
            }

            // Clear the header variable
            header = "";
            // Close the connection
            webClient.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
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
