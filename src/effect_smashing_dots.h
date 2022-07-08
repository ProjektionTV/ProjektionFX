#include <effect.h>

#if !defined(EFFECT_SMASHING_DOTS_H__)
#define EFFECT_SMASHING_DOTS_H__

class EffectSmashingDots : public Effect
{
private:
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // speed 2 = flash on every beat
        int frame = beatInfo.animationFrame(2);

        // turn off all leds
        leds = CRGB::Black;

        // width of the bars
        int width = numLeds / 10;

        // brightness of the dots
        int ramp = map(frame < 500 ? 499 - frame : 999 - frame, 0, 499, 4, 16); // negative sawtooth for flashing
        int brightness = ramp*ramp - 1; // square to fade quickly from 255 to 15

        // colors
        int byteFrame = map(beatInfo.animationFrame(16), 0, 999, 0, 255);
        CHSV barColor = CHSV(byteFrame, 255, 40);
        CHSV dotColor = CHSV((byteFrame + 128) % 256, 255, brightness);

        // draw the bars
        int barAmplitude = width/2;
        int barOffset = map(frame < 500 ? frame : 999 - frame, 0, 499, -barAmplitude, barAmplitude); // up and down ramp
        leds(numLeds*1/4 - width/2 + barOffset, numLeds*1/4 + width/2-1 + barOffset) = barColor; // bar1
        leds(numLeds*3/4 - width/2 - barOffset, numLeds*3/4 + width/2-1 - barOffset) = barColor; // bar2

        // draw the dots
        int dotAmplitude = ((numLeds/2 - width - barAmplitude*2) / 2 - 1) / 2;
        int dotOffset = map(frame < 500 ? frame : 999 - frame, 0, 499, -dotAmplitude, dotAmplitude); // up and down ramp
        leds[dotAmplitude - dotOffset] = dotColor; // dot1
        leds[numLeds/2 - dotAmplitude - dotOffset - 1] = dotColor; // dot2
        leds[numLeds/2 + dotAmplitude + dotOffset] = dotColor; // dot3
        leds[numLeds - dotAmplitude + dotOffset - 1] = dotColor; // dot4
    }
};

#endif // EFFECT_SMASHING_DOTS_H__
