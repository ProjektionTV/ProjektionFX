#include <effect.h>

class EffectBlinkRainbow : Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        static unsigned long lastBeat;
        static unsigned long lastFade;

        CHSV color = CHSV(map(beatInfo.animationFrame(8), 0, 999, 0, 255), 255, map(beatInfo.animationFrame(1), 0, 999, 255, 0));

        for(int i = 0; i < numLeds; i++){
            leds[i] = color;
        }
    }
};