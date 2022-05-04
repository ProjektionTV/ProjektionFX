#if !defined(__PROJECTIONFX_E131SYNC_H__)
#define __PROJECTIONFX_E131SYNC_H__

#include "e131/E131.h"
#include "settings.h"
E131 e131;
class E131Sync
{
private:
    const uint16_t universeTX = 1;
    uint8_t DMXsequence = 1;
    long previousMillis = 0;

public:
    void setup()
    {
        for (int universe = 0; universe <= ceil(NUM_LEDS / 170); universe++){
            e131.begin(E131_MULTICAST, universe + universeTX);
        }
        
    }
    void loop()
    {
        // according to https://github.com/Aircoookie/WLED/issues/2417#issuecomment-995487832
        // and https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#limitations
        // send might fail, if rate is to high;
        // therefore -> limit to 60fps
        if (millis() - previousMillis >= (1000 / 30))
        {
            previousMillis = millis();
            DMXsequence++;
            if (DMXsequence > 0xFF)
                DMXsequence = 1;
            // e131.clearSendBuffer();
            e131.setSequenceNumber(DMXsequence);
            e131.setSourceName("ProjektionFX");
            
            
            uint16_t MAX_PER_UNIVERSE = 170;

            // Serial.printf("looping e131 - %d %d\n", ceil(NUM_LEDS / MAX_PER_UNIVERSE), NUM_LEDS);

            for (int universe = 0; universe <= ceil(NUM_LEDS / MAX_PER_UNIVERSE); universe++)
            {
                // Serial.printf("Sending universe %d\n", universe + universeTX);

                for (int i = 0; i < MAX_PER_UNIVERSE; i++)
                {
                    int ledPos = i + (MAX_PER_UNIVERSE * (universe));
                    // Serial.printf(" - LED %d\n", ledPos);
                    if (ledPos < NUM_LEDS)
                    {
                        CRGB led = leds[ledPos];
                        e131.setRGB(i * 3, led.r, led.g, led.b);
                    } else {
                        e131.setRGB(i * 3, 0, 0, 0);
                    }
                }

                e131.sendPacket(universe + universeTX);
            }

            // while (ledsLeft > 0)
            // {
            //     // e131.clearSendBuffer();
            //     Serial.printf("Sending %d - start %d - universe %d\n", min(MAX_PER_UNIVERSE, ledsLeft), universesSent * MAX_PER_UNIVERSE, universe);
            //     for (uint16_t i = 0; i < 170; i++)
            //     {
            //         Serial.printf("-> setRGB(%d)\n", i * 3);
            //         if (i < min(MAX_PER_UNIVERSE, ledsLeft))
            //         {
            //             CRGB led = leds[i + (universesSent * MAX_PER_UNIVERSE)];
            //             e131.setRGB(i * 3, led.r, led.g, led.b); // not sure about the order; seems strange, but fits for me.
            //         }
            //         else
            //         {
            //             e131.setRGB(i * 3, 255, 0, 0); // not sure about the order; seems strange, but fits for me.
            //         }
            //     }

            //     e131.sendPacket(1);
            //     ledsLeft -= (MAX_PER_UNIVERSE * 3);
            //     universesSent++;
            // }
        }
    }
};

static E131Sync e131sync;

#endif // __PROJECTIONFX_E131SYNC_H__