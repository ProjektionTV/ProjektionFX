#include <effect.h>

#if !defined(EFFECT_STROBO_RAINBOW_H__)
#define EFFECT_STROBO_RAINBOW_H__

class EffectStroboRainbow : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        static bool flashFlag = false;

        CHSV color(map(beatInfo.animationFrame(4),0,999,0,255),255,255);

        EVERY_N_MILLIS(1000 / 15)
        {
            flashFlag = !flashFlag;
        }

        if(flashFlag)
        {
            leds = color;
        }
        else
        {
            FastLED.clearData();   
        }
    }
};

#endif // EFFECT_STROBO_RAINBOW_H__
