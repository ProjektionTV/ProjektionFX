#include <Arduino.h>
#include <FastLED.h>

// #include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ArduinoOTA.h>

#include "BeatInfo.h"
#include "settings.h"

#include "mqtt.h"

#include "effects.h"

CRGBArray<NUM_LEDS> leds;

BeatInfo beatInfo;

int currentEffect = PFX_BLINK_RAINBOW;

WiFiManager wifiManager;

void setup()
{

  Serial.begin(115200);

  wifiManager.autoConnect("ProjektionFX");

  ArduinoOTA.begin();
  setupMqtt();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{

  EVERY_N_MILLIS(50)
  {
    loopMqtt();
    ArduinoOTA.handle();
  }

  beatInfo.loop();

  switch (currentEffect)
  {
  case PFX_BLINK_RAINBOW:
    EffectBlinkRainbow::run(beatInfo, leds, NUM_LEDS);
    break;
  case PFX_MOVING_DOT:
    EffectMovingDot::run(beatInfo, leds, NUM_LEDS);
    break;
  case PFX_MOVING_DOT_SIMPLE:
    EffectMovingDotSimple::run(beatInfo, leds, NUM_LEDS);
    break;
  default:
    break;
  }

  FastLED.show();

  EVERY_N_SECONDS(30)
  {
    currentEffect = (currentEffect + 1) % EFFECTS_TOTAL;
    Serial.printf("current effect: %i", currentEffect);
  };

  // debug only :)
  // EVERY_N_SECONDS(10)
  // {
  //   beatInfo.changeBPM();
  // };
}