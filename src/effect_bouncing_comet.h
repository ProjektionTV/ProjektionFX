#include <effect.h>

#if !defined(EFFECT_BOUNCING_COMET_H__)
#define EFFECT_BOUNCING_COMET_H__

class EffectBouncingComet : public Effect
{
private:
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // speed 4 = bounce on every 2nd beat
        int frame = beatInfo.animationFrame(4);

        // turn off all leds
        leds = CRGB::Black;

        // width of the comet
        int width = numLeds / 6;

        // position of the comet
        int position = map(frame < 500 ? frame : 999 - frame, 0, 499, 0, numLeds - 1); // up and down ramp
        int tail;

        // direction and color of the comet
        static int direction = 1; // 1 = forward, -1 = backward
        static int color = rand() % 256; // start with random color

        // calculate the comet and tail
        if (frame < 500) // moving forward
        {
            if (direction == -1) // bounce: change direction and color
            {
                direction = 1;
                color = (color + 96) % 256; // loop through 8 different colors with big steps
            }
            tail = max(position - (width - 1), 0); // limit tail to range of 0 to numLeds - 1
        } 
        else // moving backward
        {
            if (direction == 1) // bounce: change direction and color
            {
                direction = -1;
                color = (color + 96) % 256; // loop through 8 different colors with big steps
            }
            tail = min(position + (width - 1), (numLeds - 1)); // limit tail to range of 0 to numLeds - 1
        }

        // gradient from full to 10/255th, going from position to tail 
        leds(position, tail).fill_gradient(CHSV(color, 255, 255), CHSV(color, 255, 100), CHSV(color, 255, 10));
    }
};

#endif // EFFECT_BOUNCING_COMET_H__
