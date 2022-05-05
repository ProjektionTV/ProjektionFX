#if !defined(EFFECTS_H__)
#define EFFECTS_H__

#include "effect_movingdot.h"
#include "effect_blink_rainbow.h"
#include "effect_movingdot_simple.h"
#include "effect_beating_rainbow_stripes.h"
#include "effect_breath_center.h"
#include "effect_quarter_beat_11.h"
#include "effect_quarter_beat_14.h"


#define EFFECTS_TOTAL 7

// List of effects
#define PFX_MOVING_DOT 0
#define PFX_BLINK_RAINBOW 1
#define PFX_MOVING_DOT_SIMPLE 2
#define PFX_BEATING_RAINBOW_STRIPES 3
#define PFX_BREATH_CENTER 4
#define PFX_QUARTER_BEAT_11 5
#define PFX_QUARTER_BEAT_14 6


extern CRGBArray<NUM_LEDS> leds;

class EffectsRunner
{
private:
    int currentEffect = PFX_QUARTER_BEAT_11;

public:
    void run()
    {
        switch (currentEffect)
        {
        default:
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
        case PFX_BREATH_CENTER:
            EffectBreathCenter::run(beatInfo, leds, NUM_LEDS);
            break;
        case PFX_QUARTER_BEAT_11:
            EffectQuarterBeat11::run(beatInfo, leds, NUM_LEDS);
            break;
        case PFX_QUARTER_BEAT_14:
            EffectQuarterBeat14::run(beatInfo, leds, NUM_LEDS);
            break;
        }
    }
    int getCurrentEffect()
    {
        return currentEffect;
    }
    void setEffect(int effect)
    {
        if (effect >= 0 && effect < EFFECTS_TOTAL)
            currentEffect = effect;
    }
    void nextEffect()
    {
        currentEffect = (currentEffect + 1) % EFFECTS_TOTAL;
    }
};

EffectsRunner effectsRunner;

#endif // EFFECTS_H__
