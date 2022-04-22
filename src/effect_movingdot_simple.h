#include <effect.h>

#if !defined(EFFECT_MOVINGDOT_SIMPLE_H__)
#define EFFECT_MOVINGDOT_SIMPLE_H__

class EffectMovingDotSimple : public Effect
{
public:
    static void run(BeatInfo beatInfo, CRGBSet leds, int numLeds)
    {

        leds.fadeToBlackBy(100);

        int redLED = map(beatInfo.animationFrame(1), 0, 1000, 0, numLeds);
        leds[redLED] = CRGB::Red;

        int greenLED = map(beatInfo.animationFrame(2), 0, 1000, 0, numLeds);
        leds[greenLED] = CRGB::Green;

        blur1d(leds, numLeds, 64);
    }
};

#endif // EFFECT_MOVINGDOT_SIMPLE_H__
