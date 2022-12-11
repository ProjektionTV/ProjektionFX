#include <effect.h>

#if !defined(EFFECT_MOVINGDOT_H__)
#define EFFECT_MOVINGDOT_H__

class EffectMovingDot : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {

        leds = CRGB::Black;


        // first dot: 
        // - length of up to 50 pixels
        // - color -> rainbow; full spectrum within 32 beats, blended to black
        // - position: full led strip within 1 beat
        CHSV color1 = CHSV(map(beatInfo.animationFrame(32), 0, 999, 0, 255),
                           255,
                           160);

        int length = limitMinimumAndMaximum(numLeds/divider50, minimumLeds, maximumLedsForRandomColorBar);
        int ledPos = map(beatInfo.animationFrame(1), 0, 999, 0, numLeds-1);

        for (int i = 0; i < length; i++)
        {
            int p = (ledPos + i) % numLeds;
            leds[p] = color1;
            leds[p].fadeToBlackBy((length - i) * (255. / length));
        }


        // second dot:
        // - length of up to 10 pixels
        // - position: moves in reverse within 8 beats
        // - color: fixed to red, blended to black
        int redLed = map(beatInfo.animationFrame(8), 0, 999, numLeds-1, 0);
    
        length = limitMinimumAndMaximum(numLeds/divider10, minimumLeds, maximumLedsForRedColorBar);
        for (int i = 0; i < length; i++)
        {
            int p = (redLed + i) % numLeds;
            leds[p] = blend(leds[redLed], CRGB::Red, 30);
            leds[p].fadeToBlackBy((i) * (255. / length));
        }

    }

private:
    const static int divider50 = 144 / 50;
    const static int divider10 = 144 / 10;
    const static int minimumLeds = 2;
    const static int maximumLedsForRandomColorBar = 50;
    const static int maximumLedsForRedColorBar = 10;

    static int limitMinimumAndMaximum(int value, int minimumLimit, int maximumLimit){
        if(value < minimumLimit){
            return minimumLimit;
        }
        if(maximumLimit < value){
            return maximumLimit;
        }
        return value;
    }
};

#endif // EFFECT_MOVINGDOT_H__
