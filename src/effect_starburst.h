#include <effect.h>

#if !defined(EFFECT_STARBURST_H__)
#define EFFECT_STARBURST_H__

class EffectStarburst : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // Der Effekt läuft von der Mitte des LED-Streifens aus nach aussen
        const static int halfLedCount = numLeds / 2;

        // Je weiter aussen der Effekt ist desto weniger LEDs sollen leuchten.
        const static int achtel = numLeds / 8;

        // in jedem Durchlauf alle LEDs werden etwas dunkler
        leds.fadeToBlackBy(255);

        // aktuelle Position auf dem halben Streifen
        int currentPosition = map(beatInfo.animationFrame(1), 0, 999, 0, halfLedCount);
        // Farbe abhängig vom Beat
        uint8_t hue = map(beatInfo.animationFrame(16), 0, 999, 0, 255);

        // LEDs einschalten
        for (int l = max(0, currentPosition - achtel); l < currentPosition; l++) {
            int mod = l / achtel + 2;

            // je weiter aussen auf dem Streifen desto weniger LEDs an
            if (l % mod == 0) {
                leds[halfLedCount + l] = CHSV(hue, 255, 128);
                leds[halfLedCount - l] = CHSV(hue, 255, 128);
            }
        }
    }
};

#endif // EFFECT_STARBURST_H__