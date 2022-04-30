#include <effect.h>

#if !defined(EFFECT_BREATHCENTER_H__)
#define EFFECT_BREATHCENTER_H__

class EffectBreathCenter : public Effect
{
private:
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // breath speed: every beat
        int frame = beatInfo.animationFrame(1);

        // turn of all leds
        leds = CRGB::Black;

        int byteFrame = map(frame, 0, 999, 0, 255);

        // width of the lightended leds, min 10% of numleds
        int width = map(sin8(byteFrame), 0, 255, numLeds * 0.1, numLeds-1);

        // center position
        int ledCenter = numLeds / 2;

        // rotate color; full rotation at 16 beats
        int color = map(beatInfo.animationFrame(16), 0, 999, 0, 255);

        // gradient from 10/255th to full to 10/255th for the calculated width, starting at center
        leds(ledCenter - width / 2, ledCenter + width / 2).fill_gradient(CHSV(color, 255, 10), CHSV(color, 255, 255), CHSV(color, 255, 10));
    }
};

#endif // EFFECT_BREATHCENTER_H__
