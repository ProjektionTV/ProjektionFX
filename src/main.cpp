#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <https_web_server.h>
#include <StreamInfos.h>
#include <ESPNtpClient.h>

#include "BeatInfo.h"
#include "settings.h"
#include "mqtt.h"
#include "configuration.h"
#include "effects.h"
#include "e131sync.h"
#include "version.h"

CRGBArray<MAX_NUM_LEDS> leds;

BeatInfo beatInfo;

HttpsWebServer https;

extern int8_t nextEffectNumber;
extern const char* nextEffectName;
extern int64_t nextEffectTimestampUs;

extern Configuration config;

void setup()
{
  Serial.begin(115200);
  Serial.printf("\n\nProjektionFX v%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

  bool configPortal = false;
  pinMode(0, INPUT);
  Serial.println("Press BootButton for ConfigPortal.");
  delay(2500);
  if (!digitalRead(0))
      configPortal = true;

  config.setupWifiPortal("ProjektionFX", configPortal);
  
  ArduinoOTA.begin();

  setupMqtt(config.getMQTTHost());

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, config.getNumLeds());
  FastLED.setMaxPowerInVoltsAndMilliamps(5, config.getMaxMilliamps());

  https.setupDNS();
  https.setSSLCert();
  https.start();

#ifdef E131_ENABLED
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
  int64_t microsTillNextEffect = currentMicros - (nextEffectTimestampUs + (streamInfos.latency * 1000));
  if (nextEffectTimestampUs > 0 && microsTillNextEffect >= 0)
  {
    Serial.printf("Set next Effect: %d\n", nextEffectNumber);
    nextEffectTimestampUs = -1;
    effectsRunner.setEffect(nextEffectNumber);
  }
  /*else
  {
    EVERY_N_SECONDS(30)
    {
      effectsRunner.nextEffect();
    };
  }*/

  // Update the time for the beat calculation
  beatInfo.loop();
  effectsRunner.run();
  FastLED.show();

#ifdef E131_ENABLED
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