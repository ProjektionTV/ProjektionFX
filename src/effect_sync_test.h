#include <effect.h>

#if !defined(EFFECT_SYNCTEST_H__)
#define EFFECT_SYNCTEST_H__

class EffectSyncTest : public Effect
{
public:
    static void run(BeatInfo &beatInfo, CRGBSet leds, int numLeds)
    {

        leds = CRGB::Black;

        int x = beatInfo.animationFrame(32);

        if (x < 333)
        {
            leds.fill_rainbow(0);
        }
        else if (x < 666)
        {
            int y = beatInfo.animationFrame(2);
            for (int i = 0; i < numLeds; i++)
            {

                if (y < 500)
                {
                    if (i % 2 == 0)
                    {
                        leds[i] = CRGB::Red;
                    } else {
                        leds[i] = CRGB::White;
                    }
                } else {
                    if (i % 2 == 1)
                    {
                        leds[i] = CRGB::Red;
                    } else {
                        leds[i] = CRGB::White;
                    }
                }
            }
        }
        else
        {
            leds.fill_rainbow(map(x, 0, 1000, 0, 255), 20);
        }
    }
};

#endif // EFFECT_SYNCTEST_H__
