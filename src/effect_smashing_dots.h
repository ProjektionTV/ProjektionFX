#include <effect.h>

#if !defined(EFFECT_SMASHING_DOTS_H__)
#define EFFECT_SMASHING_DOTS_H__

class EffectSmashingDots : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // speed 2 = flash on every beat
        int frame = beatInfo.animationFrame(2);

        // turn off all leds
        leds = CRGB::Black;

        // width of the bars
        int width = limitMinimum(numLeds / 10, minimumLedsForBar);

        // brightness of the dots
        int ramp = map(frame < 500 ? 499 - frame : 999 - frame, 0, 499, 8, 16); // negative sawtooth for flashing
        int brightness = ramp*ramp - 1; // square to fade quickly from 255 to 63

        // colors
        int byteFrame = map(beatInfo.animationFrame(16), 0, 999, 0, 255);
        CHSV barColor = CHSV(byteFrame, 255, 100);
        CHSV dotColor = CHSV((byteFrame + 128) % 256, 255, brightness);

        // draw the bars
        int barRange = width;
        int barOffset = map(frame < 500 ? frame : 999 - frame, 0, 499, 0, barRange); // up and down ramp
        leds(numLeds*1/4 - barOffset, numLeds*1/4 + barRange - barOffset - 1) = barColor; // bar1
        leds(numLeds*3/4 - barRange + barOffset, numLeds*3/4 + barOffset - 1) = barColor; // bar2

        // draw the dots
        int dotRange = limitMinimum(numLeds/4, minimumForDotRange) - width - 1;
        int dotOffset = map(frame < 500 ? frame : 999 - frame, 0, 499, 0, dotRange); // up and down ramp
        leds[dotOffset] = dotColor; // dot1
        leds[numLeds/2 - dotRange + dotOffset - 1] = dotColor; // dot2
        leds[numLeds/2 + dotRange - dotOffset] = dotColor; // dot3
        leds[numLeds - dotOffset - 1] = dotColor; // dot4
    }

private:
    const static int minimumLedsForBar = 2;
    const static int minimumForDotRange = 4;

    static int limitMinimum(int value, int minimumLimit){
        if(value < minimumLimit){
            return minimumLimit;
        }
        return value;
    }
};

#endif // EFFECT_SMASHING_DOTS_H__
