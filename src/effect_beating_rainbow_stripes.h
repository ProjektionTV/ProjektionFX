#include <effect.h>

#if !defined(EFFECT_BEATINGRAINBOWSTRIPES_H__)
#define EFFECT_BEATINGRAINBOWSTRIPES_H__

class EffectBeatingRainbowStripes : public Effect
{
public:
    static double easeInOutElastic(double t)
    {
        double t2;
        if (t < 0.45)
        {
            t2 = t * t;
            return 8 * t2 * t2 * sin(t * PI * 9);
        }
        else if (t < 0.55)
        {
            return 0.5 + 0.75 * sin(t * PI * 4);
        }
        else
        {
            t2 = (t - 1) * (t - 1);
            return 1 - 8 * t2 * t2 * sin(t * PI * 9);
        }
    }

    static double easeInOutBounce(double t)
    {
        if (t < 0.5)
        {
            return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * PI * 7));
        }
        else
        {
            return 1 - 8 * pow(2, -8 * t) * abs(sin(t * PI * 7));
        }
    }
    static float mapf(long x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        leds.fadeToBlackBy(20);

        int color = map(beatInfo.animationFrame(4), 0, 999, 0, 255);
        // int brightness = sin8(map(beatInfo.animationFrame(4), 0, 999, 0, 255));

        float bF = mapf(beatInfo.animationFrame(2), 0, 999, -1.0, 1.0);
        int brightness = map(easeInOutElastic(bF) * 100, 0, 100, 0, 255);

        for (int i = 4; i <= 32; i *= 2)
        {
            int beat = sin8(map(beatInfo.animationFrame(i), 0, 999, 0, 255));
            int led = map(beat, 0, 255, 0, numLeds-1);
            leds[led] += CHSV(color, 255, map(brightness, 0, 255, 64, 255));
        }

        for (int i = 4; i <= 32; i *= 2)
        {
            int beat = sin8(map(beatInfo.animationFrame(i), 0, 999, 0, 255));
            int led = map(beat, 0, 255, numLeds-1, 0);
            leds[led] += CHSV(color, 255, map(brightness, 0, 255, 64, 255));
        }

        blur1d(leds, numLeds, 64);
    }
};

#endif // EFFECT_BEATINGRAINBOWSTRIPES_H__
