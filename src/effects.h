#if !defined(EFFECTS_H__)
#define EFFECTS_H__

#include "effect_movingdot.h"
#include "effect_blink_rainbow.h"
#include "effect_movingdot_simple.h"
#include "effect_beating_rainbow_stripes.h"

#define EFFECTS_TOTAL 4

// List of effects
#define PFX_MOVING_DOT 0
#define PFX_BLINK_RAINBOW 1
#define PFX_MOVING_DOT_SIMPLE 2
#define PFX_BEATING_RAINBOW_STRIPES 3

extern CRGBArray<NUM_LEDS> leds;

class EffectsRunner
{
private:
    int currentEffect = PFX_BEATING_RAINBOW_STRIPES;

public:
    void run()
    {
        switch (currentEffect)
        {
        case PFX_MOVING_DOT:
            EffectMovingDot::run(beatInfo, leds, NUM_LEDS);
            break;
        case PFX_BLINK_RAINBOW:
            EffectBlinkRainbow::run(beatInfo, leds, NUM_LEDS);
            break;
        case PFX_MOVING_DOT_SIMPLE:
            EffectMovingDotSimple::run(beatInfo, leds, NUM_LEDS);
            break;
        case PFX_BEATING_RAINBOW_STRIPES:
            EffectBeatingRainbowStripes::run(beatInfo, leds, NUM_LEDS);
            break;
        default:
            break;
        }
    }
    int getCurrentEffect()
    {
        return currentEffect;
    }
    void setEffect(int effect)
    {
        if (effect > 0 && effect < EFFECTS_TOTAL)
            currentEffect = effect;
    }
    void nextEffect()
    {
        currentEffect = (currentEffect + 1) % EFFECTS_TOTAL;
    }
};

EffectsRunner effectsRunner;

#endif // EFFECTS_H__
