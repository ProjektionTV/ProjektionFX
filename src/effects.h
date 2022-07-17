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
#include "effect_bouncing_comet.h"
#include "effect_dancing_spotlights.h"
#include "effect_smashing_dots.h"

extern CRGBArray<NUM_LEDS> leds;
extern BeatInfo beatInfo;

typedef void EffectFunction(BeatInfo&, CRGBSet, int);

#define EFFECT_ENTRY(NAME) { NAME::run, #NAME }
#define EMPTY_EFFECT_ENTRY() { nullptr, "empty" }
struct EffectEntry{
    EffectFunction* effectFunction;
    const char* effectName;
};

class EffectsRunner
{
private:
    int currentEffect = 0;
    static constexpr const EffectEntry effects[] = {
        EFFECT_ENTRY(EffectMovingDot),
        EFFECT_ENTRY(EffectBlinkRainbow),
        EFFECT_ENTRY(EffectMovingDotSimple),
        EFFECT_ENTRY(EffectBeatingRainbowStripes),
        EFFECT_ENTRY(EffectBreathCenter),
        EFFECT_ENTRY(EffectQuarterBeat11),
        EFFECT_ENTRY(EffectQuarterBeat14),
        EMPTY_EFFECT_ENTRY(), // PFX_ILJA1
        EMPTY_EFFECT_ENTRY(), // PFX_ILJA2
        EFFECT_ENTRY(EffectStarburst),
        EFFECT_ENTRY(EffectBouncingComet),
        EFFECT_ENTRY(EffectDancingSpotlights),
        EFFECT_ENTRY(EffectSmashingDots),
    };
    int availableEffects(){
        return sizeof(effects)/sizeof(*effects);
    }

public:
    void run()
    {
        Serial.printf("Current effect %s(%d)\n", effects[currentEffect].effectName, currentEffect);

        EffectFunction* effectFunction = effects[currentEffect].effectFunction;
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
    const char* getCurrentEffectName()
    {
        return effects[currentEffect].effectName;
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
constexpr const EffectEntry EffectsRunner::effects[];

#endif // EFFECTS_H__
