#if !defined(__PROJEKTIONTX_MQTT_H__)
#define __PROJEKTIONTX_MQTT_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "settings.h"
#include "configuration.h"

#include <BeatInfo.h>

extern BeatInfo beatInfo;

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqttUser;
const char *mqttPassword;

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

void reconnect()
{
  if(!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "PROJEKTIONFX-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect

    if (client.connect(clientId.c_str(), config.getMQTTUser(), config.getMQTTPassword()))
    {
      Serial.println("connected");
      client.subscribe("projektiontv/stream/dj/songinfo/bpm");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void processBpm(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "projektiontv/stream/dj/songinfo/bpm") == 0)
  {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, payload, length);

    double bpm = doc["bpm"];
    Serial.printf("MQTT Received BPM: %f\n", bpm);
    beatInfo.setBPM(bpm);
  }
}

void processNames(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "projektiontv/stream/dj/songinfo/names") == 0)
  {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, payload, length);

    const char* artistname = doc["artistname"];
    const char* songname = doc["songname"];
    Serial.printf("MQTT Received Names: %s - %s\n", artistname, songname);
  }
}

void processEffect(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "projektiontv/stream/dj/effect") == 0)
  {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, payload, length);

    uint8_t number = doc["number"];
    const char* name = doc["name"];
    uint64_t timestamp_us = doc["timestamp_us"];

    Serial.printf("MQTT Received Effect: number=%d name=%s timestamp_us=%d \n", number, name, timestamp_us);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  processBpm(topic, payload, length);
  processNames(topic, payload, length);
  processEffect(topic, payload, length);
}

void setupMqtt(const char *host)
{

  Serial.println("Setting Up MQTT");
  Serial.printf("- connect to %s\n", host);
  client.setServer(host, 1883);

  client.setCallback(callback);
  reconnect();

  Serial.println("MQTT Setup completed!");
}

void loopMqtt()
{
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();
}

#endif // __PROJEKTIONTX_MQTT_H__
