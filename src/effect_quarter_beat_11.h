#include <effect.h>

#if !defined(EFFECT_QUARTER_BEAT11_H__)
#define EFFECT_QUARTER_BEAT11_H__

class EffectQuarterBeat11 : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        static uint8_t oldQuarter = 0;
        FastLED.clearData();
        uint8_t quarter = beatInfo.animationFrame(1) / 250;
        static u_int8_t hue = 0;
        
        if(oldQuarter != quarter)
        {
            oldQuarter = quarter;
            hue = random(255);
        }

        for(int i=0; i<(NUM_LEDS/4);i++)
        {
            leds[i+(quarter*(NUM_LEDS/4))] = CHSV(hue, 255, 128);
        }
    }
};

#endif // EFFECT_QUARTER_BEAT11_H__
