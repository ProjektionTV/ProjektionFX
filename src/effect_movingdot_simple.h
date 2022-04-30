#include <effect.h>

#if !defined(EFFECT_MOVINGDOT_SIMPLE_H__)
#define EFFECT_MOVINGDOT_SIMPLE_H__

class EffectMovingDotSimple : public Effect
{
public:
    static void run(BeatInfo& beatInfo, CRGBSet leds, int numLeds)
    {
        // alle LEDs werden etwas "ausgeschaltet", also dunkler
        leds.fadeToBlackBy(100); 

        // ein rotes Licht, dass auf jeden Beat einmal komplett über alle LEDs läuft
        // als erstes wird die Position bestimmt:
        // - die Zeit wo wir uns befinden, wird über die Methode beatInfo.animationFrame(1) abgefragt und 
        //   gibt einen Wert von 0 (= bedeutet, wir sind ganz am Anfang, also der Beat kam gerade) bis 999 
        //   (das heißt wir sind ganz am Ende, der nächste Beat kommt gleich!) zurück
        // - die map()-Methode 'mappt' (Dreisatz!) die Werte von animationFrame (0-999) auf den Bereich von
        //   0 bis zu unserer letzten LED (=numLeds)
        // - das Ergebnis wird in der Variable redLEDPosition gespeichert
        int redLEDPosition = map(beatInfo.animationFrame(1), 0, 999, 0, numLeds-1);

        // nun schalten wir die LED an der Position redLEDPosition auf rot
        leds[redLEDPosition] = CRGB::Red;

        // ein grünes Licht, dass in der Zeit von zwei Beat einmal komplett über alle LEDs läuft:
        // genauso, wie bei der roten, außer das wir hier beatInfo.animationFrame(2) aufrufen,
        // d.h.:
        // - bei dem Rückgabewert 0 sind wir auf dem 1. Beat,
        // - bei dem Rückgabewert 500 sind wir auf dem 2. Beat
        // - bei dem Wert 999 sind wir wieder ganz kurz vor dem 1. Beat
        // Insgesamt braucht die grüne LED immer 2 Beats (bei 120BPM wäre das 1 Sekunde), um über den 
        // gesamten LED Streifen zu laufen.
        int greenLEDPosition = map(beatInfo.animationFrame(2), 0, 999, 0, numLeds-1);
        leds[greenLEDPosition] = CRGB::Green;


        // zum Abschluss noch ein bisschen Weichzeichnen; ist nicht notwendig, aber etwas 'schöner' und weicher,
        // wenn der Stripe lang ist. Denn auf jeden Beat über viele LEDs laufen ist schon ganz schön schnell ;)
        blur1d(leds, numLeds, 64);
    }
};

#endif // EFFECT_MOVINGDOT_SIMPLE_H__
