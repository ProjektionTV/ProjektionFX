#if !defined(EFFECT_H__)
#define EFFECT_H__

#include <Arduino.h>
#include <settings.h>
#include <FastLED.h>
#include <BeatInfo.h>

class Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGB leds[], int numLeds);
};

#endif // EFFECT_H__

