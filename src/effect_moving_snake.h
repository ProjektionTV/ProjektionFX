#include <effect.h>

#if !defined(EFFECT_MOVING_SNAKE_H__)
#define EFFECT_MOVING_SNAKE_H__

CRGB colors[8] = {
  CRGB(255, 0, 0), // br = 23
  CRGB(255, 36, 0), // br = 23
  CRGB(255, 0, 15), // br 23
  CRGB(0, 98, 0), // br 23
  CRGB(0, 0, 126), // br 23
  CRGB(0, 67, 64), // br 23
  CRGB(255, 0, 12), // br 23
  CRGB(255, 255, 255) // 23
};

class EffectMovingSnake : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        static int length = numLeds/8;
        static int oldFrame = beatInfo.animationFrame(16);
        static CRGB color = CRGB::Red;

        if(beatInfo.animationFrame(16) < oldFrame)
        {
            color = colors[random(8)];
        }
        oldFrame = beatInfo.animationFrame(16);

        FastLED.clearData();

        int ledPos = map(beatInfo.animationFrame(1), 0, 999, 0, numLeds-1);

        for (int i = 0; i < length; i++)
        {
            int p = min(numLeds-1, max(0, ledPos + i));
            leds[p] = color;
            leds[p].fadeToBlackBy((length - i) * (255. / length));
        }
    }
};

#endif // EFFECT_MOVING_SNAKE_H__
