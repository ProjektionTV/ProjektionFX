#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>
// #include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <https_web_server.h>
#include <ESPNtpClient.h>

#include "BeatInfo.h"
#include "settings.h"
#include "mqtt.h"
#include "configuration.h"
#include "effects.h"
#include "artnet.h"
#include "e131sync.h"

CRGBArray<NUM_LEDS> leds;

BeatInfo beatInfo;

HttpsWebServer https;

extern int8_t nextEffectNumber;
extern const char* nextEffectName;
extern int64_t nextEffectTimestampUs;
extern uint64_t streamLatency;

void setup()
{

  Serial.begin(115200);

  config.setupWifiPortal("ProjektionFX");

  ArduinoOTA.begin();

  setupMqtt(config.getMQTTHost());

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_MAX_MILLIAMP);

  https.setupDNS();
  https.generateSSLCert();
  https.start();

#ifdef ARTNET_ENABLED
  e131sync.setup();
#endif

  NTP.setTimeZone (TZ_Etc_GMTp1);
  NTP.begin ();
}

void loop()
{
  config.connectionGuard();
  
  // Provide player and update latency
  https.loop();

  // Receive the data from MQTT broker
  loopMqtt();

  ArduinoOTA.handle();

  // Check time till next effect if given
  int64_t currentMicros = NTP.micros();
  int64_t microsTillNextEffect = currentMicros - (nextEffectTimestampUs + https.streamLatency);
  if (nextEffectTimestampUs > 0 && microsTillNextEffect >= 0 && microsTillNextEffect < 10)
  {
    effectsRunner.setEffect(nextEffectNumber);
  }
  else
  {
    EVERY_N_SECONDS(30)
    {
      effectsRunner.nextEffect();
    };
  }
  // Update the time for the beat calculation
  beatInfo.loop();
  effectsRunner.run();
  FastLED.show();

#ifdef ARTNET_ENABLED
  e131sync.loop();
#endif
  /*
  // debug only :)
  EVERY_N_SECONDS(10)
  {
    beatInfo.changeBPM();
  };
  */
}