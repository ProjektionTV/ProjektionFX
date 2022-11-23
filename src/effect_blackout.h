#include <effect.h>

#if !defined(EFFECT_BLACKOUT_H__)
#define EFFECT_BLACKOUT_H__

class EffectBlackOut : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        FastLED.clear();
    }
};

#endif // EFFECT_BLACKOUT_H__