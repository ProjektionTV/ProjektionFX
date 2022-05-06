#include <Arduino.h>
#if defined(ESP8266)
 #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>
#endif

#include <FastLED.h>



// #include "server.h"
// // #include <ArduinoOTA.h>

#include "BeatInfo.h"
#include "settings.h"

#include "mqtt.h"
#include "configuration.h"

#include "effects.h"

// // #include "artnet.h"

#include "e131sync.h"

#include "web.h"

CRGBArray<2000> leds;

BeatInfo beatInfo;

void setup()
{

  Serial.begin(115200);

  config.setupWifiPortal("ProjektionFX");

  // // ArduinoOTA.begin();

  setupMqtt(config.getMQTTHost());

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, config.getNumLEDs());
  FastLED.setMaxPowerInVoltsAndMilliamps(5, LED_MAX_MILLIAMP);

  effectsRunner.setup();

  setupWebServer();

#ifdef E131_ENABLED
  e131sync.setup();
#endif
}

void loop()
{
  config.connectionGuard();
  loopMqtt();
  // ArduinoOTA.handle();

  beatInfo.loop();

  effectsRunner.run();

  FastLED.show();

#ifdef E131_ENABLED
  e131sync.loop();
#endif

  EVERY_N_SECONDS(30)
  {
    effectsRunner.nextEffect();
  };

  // debug only :)
  // EVERY_N_SECONDS(10)
  // {
  //   beatInfo.changeBPM();
  // };
}