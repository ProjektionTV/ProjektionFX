#if !defined(__PROJECTIONFX_E131SYNC_H__)
#define __PROJECTIONFX_E131SYNC_H__

#include "e131/E131.h"
#include "settings.h"
E131 e131;
class E131Sync
{
private:
    uint16_t universeTX = 1;
    uint8_t DMXsequence = 1;
    long previousMillis = 0;

public:
    void setup()
    {
    }
    void loop()
    {
        // according to https://github.com/Aircoookie/WLED/issues/2417#issuecomment-995487832 
        // and https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#limitations
        // send might fail, if rate is to high;
        // therefore -> limit to 60fps
        if (millis() - previousMillis >= (1000/30))
        {
            previousMillis = millis();
            if (DMXsequence > 0xFF)
                DMXsequence = 1;
            e131.setSequenceNumber(DMXsequence++);
            e131.setSourceName("ProjektionFX");

            int ledsLeft = NUM_LEDS;
            int universesSent = 0;
            int universe = universeTX;
            while (ledsLeft > 0)
            {
                Serial.printf("Sending %d - start %d - universe %d\n", min(170, ledsLeft), universesSent * 170, universe);
                for (int i = 0; i < min(170, ledsLeft); i++)
                {
                    CRGB led = leds[i + (universesSent * 170)];
                    e131.setRGB(i * 3, led.r, led.g, led.b); // not sure about the order; seems strange, but fits for me.
                }
                ledsLeft -= min(170, ledsLeft);

                e131.sendPacket(universe++);
                universesSent++;
            }
            Serial.println("");
        }
    }
};

static E131Sync e131sync;

#endif // __PROJECTIONFX_E131SYNC_H__