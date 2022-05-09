#if !defined(EFFECTS_H__)
#define EFFECTS_H__

#include "BeatInfo.h"
#include "effect_movingdot.h"
#include "effect_blink_rainbow.h"
#include "effect_movingdot_simple.h"
#include "effect_beating_rainbow_stripes.h"
#include "effect_breath_center.h"
#include "effect_quarter_beat_11.h"
#include "effect_quarter_beat_14.h"
#include "effect_starburst.h"

extern CRGBArray<NUM_LEDS> leds;
extern BeatInfo beatInfo;

typedef void EffectFunction(BeatInfo&, CRGBSet, int);

class EffectsRunner
{
private:
    int currentEffect = 0;
    static constexpr const EffectFunction* effects[] = {
        EffectMovingDot::run,
        EffectBlinkRainbow::run,
        EffectMovingDotSimple::run,
        EffectBeatingRainbowStripes::run,
        EffectBreathCenter::run,
        EffectQuarterBeat11::run,
        EffectQuarterBeat14::run,
        nullptr, // PFX_ILJA1
        nullptr, // PFX_ILJA2
        EffectStarburst::run,
    };
    int availableEffects(){
        return sizeof(effects)/sizeof(*effects);
    }

public:
    void run()
    {
        Serial.printf("Current effect %d\n", currentEffect);

        EffectFunction* effectFunction = effects[currentEffect];
        if(effectFunction==nullptr){
            nextEffect();
        }else{
            effectFunction(beatInfo, leds, NUM_LEDS);
        }
    }
    int getCurrentEffect()
    {
        return currentEffect;
    }
    void setEffect(int effect)
    {
        if (effect >= 0 && effect < availableEffects())
        {
            currentEffect = effect;
        }
    }
    void nextEffect()
    {
        currentEffect = (currentEffect + 1) % availableEffects();
    }
};

EffectsRunner effectsRunner;
constexpr const EffectFunction* EffectsRunner::effects[];

#endif // EFFECTS_H__
