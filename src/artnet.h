
#if !defined(__PROJECTIONFX_E131__H)
#define __PROJECTIONFX_E131__H

#include "settings.h"

#ifdef ARTNET_ENABLED

// #include <ArtnetWifi.h>
#include <FastLED.h>
#include "settings.h"
#include <ArtnetnodeWifi.h>

extern CRGBArray<NUM_LEDS> leds;

class ArtnetSync
{
private:
    const String host = ARTNET_HOST;
    ArtnetnodeWifi artnet;
    long lastSend;

public:
    void setup()
    {
        lastSend = 0;

        artnet.begin(host);
        artnet.setLength(NUM_LEDS);
        artnet.setUniverse(0);
        artnet.setShortName("ProjektionFX");
        artnet.setLongName("ProjektionFX ArtnetSync");
        artnet.setPortType(0, 0x40);
    }
    void loop()
    {
        unsigned long currentMillis = millis();

        if (currentMillis - lastSend >= (1000 / 60)) // send with 60 fps
        {

            lastSend = currentMillis;

            for (int i = 0; i < NUM_LEDS; i++)
            {
                CRGB led = leds[i];
                artnet.setByte((i * 3) + 0, led.r);
                artnet.setByte((i * 3) + 1, led.g);
                artnet.setByte((i * 3) + 2, led.b);
            }

            artnet.write();
        }
    }
};

static ArtnetSync artnetSync;

#endif
#endif // __PROJECTIONFX_E131__H