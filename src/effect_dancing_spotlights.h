#include <effect.h>

#if !defined(EFFECT_DANCING_SPOTLIGHTS_H__)
#define EFFECT_DANCING_SPOTLIGHTS_H__

class EffectDancingSpotlights : public Effect
{
private:
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // movement
        int frame = beatInfo.animationFrame(1);
        static int lastFrame = frame;

        // turn off all leds
        leds = CRGB::Black;

        // brightness
        int byteFrame = map(frame, 0, 999, 0, 255); // flash on every beat
        int brightness = sin8((byteFrame + 192) % 256); // shift sinus so that frame 0 is darkest

        // change color every beat
        static int color = 0;
        if (frame < lastFrame)
        {
            color = (color + 2) % 5; // loop through 5 different colors
        } 

        // turn on all spots with current color
        for (int i=0; i<numLeds; i++)
        {
            if (i % 5 == color)
            {
                leds[i] = CHSV(color * 51, 255, brightness);
            }
        }

        // store the last value of frame
        lastFrame = frame;
    }
};

#endif // EFFECT_DANCING_SPOTLIGHTS_H__
