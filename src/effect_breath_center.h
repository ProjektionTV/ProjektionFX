#include <effect.h>

#if !defined(EFFECT_BREATHCENTER_H__)
#define EFFECT_BREATHCENTER_H__

class EffectBreathCenter : public Effect
{
private:
public:
    static double ease(double t)
    {
        return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
    }
    static void run(BeatInfo beatInfo, CRGBSet leds, int numLeds)
    {

        int frame = beatInfo.animationFrame(1);

        // int byteFrame = map(frame, 0, 1000, 0, 255);

        // float factor = ease(sin8(byteFrame) / 255.);

        leds = CRGB::Black;

        int byteFrame = map(frame, 0, 1000, 0, 255);
        int width = map(sin8(byteFrame), 0, 255, numLeds * 0.1, numLeds);

        // leds[width] = CRGB::Red;

        int ledCenter = numLeds / 2;

        int color = map(beatInfo.animationFrame(16), 0, 1000, 0, 255);

        leds(ledCenter - width / 2, ledCenter + width / 2).fill_gradient(CHSV(color, 255, 10), CHSV(color, 255, 255), CHSV(color, 255, 10));

        // for(int i = 0; i < numLeds; i++){
        //     leds[i] = CHSV(0, 255, i * (width / 255.));
        // }

        // for (int i = 0; i < numLeds; i++)
        // {
        //     int value = cos8(map(i, -numLeds / 2, numLeds / 2, 64, 255));
        //     leds[i] = CHSV(0, 255, value * factor);
        // }
    }
};

#endif // EFFECT_BREATHCENTER_H__
