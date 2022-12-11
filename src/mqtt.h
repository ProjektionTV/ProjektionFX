#if !defined(__PROJEKTIONTX_MQTT_H__)
#define __PROJEKTIONTX_MQTT_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "settings.h"
#include "configuration.h"

#include <BeatInfo.h>

extern BeatInfo beatInfo;
extern Configuration config;

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqttUser;
const char *mqttPassword;

const char *topicBpm = "projektiontv/stream/dj/songinfo/bpm";
const char *topicNames = "projektiontv/stream/dj/songinfo/names";
const char *topicEffect = "projektiontv/stream/dj/effect";

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

int8_t nextEffectNumber = 0;
const char* nextEffectName;
int64_t nextEffectTimestampUs = 0;

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
      client.subscribe(topicBpm);
      client.subscribe(topicNames);
      client.subscribe(topicEffect);
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
  if (strcmp(topic, topicBpm) == 0)
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
  if (strcmp(topic, topicNames) == 0)
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
  if (strcmp(topic, topicEffect) != 0){
    return;
  }

  DynamicJsonDocument doc(256);
  deserializeJson(doc, payload, length);

  // extract new values from json
  int8_t newNextEffectNumber = doc["number"];
  const char* newNextEffectName = doc["name"];
  int64_t newNextEffectTimestampUs = doc["timestamp_us"];

  // skip, if the effect is allready set and "active"
  if(nextEffectNumber == newNextEffectNumber && nextEffectTimestampUs != -1){
    Serial.printf(
      "MQTT SKIP received Effect: number=%d name=%s timestamp_us=%d -> this effect is already active and should trigger at %d us\n",
      newNextEffectNumber, newNextEffectName, newNextEffectTimestampUs, nextEffectTimestampUs
    );
    return;
  }

  // set values
  nextEffectNumber = newNextEffectNumber;
  nextEffectName = newNextEffectName;
  nextEffectTimestampUs = newNextEffectTimestampUs;

  Serial.printf("MQTT Received Effect: number=%d name=%s timestamp_us=%d \n", nextEffectNumber, nextEffectName, nextEffectTimestampUs);
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
