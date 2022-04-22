#include <Arduino.h>
#include <FastLED.h>

// #include <ESP8266WebServer.h>
#include <ESP_WiFiManager.h>

#include "BeatInfo.h"
#include "settings.h"

#include "mqtt.h"

#include "effects.h"

CRGBArray<NUM_LEDS> leds;

BeatInfo beatInfo;

int currentEffect = PFX_BLINK_RAINBOW;

ESP_WiFiManager wifiManager;

void setup()
{

  Serial.begin(115200);


  wifiManager.setDebugOutput(true);
  wifiManager.autoConnect();

  setupMqtt();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{
  loopMqtt();
  beatInfo.loop();

  switch (currentEffect)
  {
  case PFX_BLINK_RAINBOW:
    EffectBlinkRainbow::run(beatInfo, leds, NUM_LEDS);
    break;
  case PFX_MOVING_DOT:
    EffectMovingDot::run(beatInfo, leds, NUM_LEDS);
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