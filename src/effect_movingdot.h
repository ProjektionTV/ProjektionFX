#include <effect.h>

#if !defined(EFFECT_MOVINGDOT_H__)
#define EFFECT_MOVINGDOT_H__

class EffectMovingDot : public Effect
{
public:
    static void run(BeatInfo beatInfo, CRGBSet leds, int numLeds)
    {

        EVERY_N_MILLIS(10)
        {

            leds.fadeToBlackBy(20);
        };

        leds[map(beatInfo.animationFrame(1), 0, 1000, 0, numLeds)] = CHSV(
            map(beatInfo.animationFrame(32), 0, 1000, 0, 255),
            255,
            160);

        leds[map(beatInfo.animationFrame(8), 0, 1000, numLeds, -1)] = CRGB::Red;
    }
};

#endif // EFFECT_MOVINGDOT_H__
