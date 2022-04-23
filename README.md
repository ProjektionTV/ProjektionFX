# ProjektionFX

ProjektionFX lässt deine WS2812-LED-Stripes zu Hause im Takt von den DJ Streams von unserem Ilja von [ProjektionTV](https://www.projektion.tv/) aufleuchten!

Besuche uns gerne:
- 📺 Live auf [twitch.tv/projektiontv](https://www.twitch.tv/projektiontv/)  
Streams immer *Montag, Mittwoch und Freitag* von 16:00 Uhr bis ca. 20/21 Uhr
- 🤷‍♂️ auf unserem [Discord](https://discord.gg/92JNBJR) - dort tummeln sich viele nette und hilfsbereite ProjektionTV ZuschauerInnen herum. Die beste Anlaufstelle, wenn es Fragen gibt!
- auf der [ProjektionTV Webseite](https://www.projektion.tv/)

# Inhaltsverzeichnis

- 🔌 [Benötigte Hardware](#benötigte-hardware)
- 🛠 [Installation](#installation)
- ⌨️ [mit Entwickeln](#mit-entwickeln)

## Benötigte Hardware
- einen [ESP8266](https://amzn.to/38htUZG)* oder [ESP32](https://amzn.to/38h9P5I)* Mikrocontroller
- einen [WS2812-LED Stripe](https://amzn.to/2LL7WH3)*
- ein paar Kabel, um den ESP und den LED Stripe miteinander zu verbinden
- *Optional:* Wenn dein LED Stripe lang ist, dann solltest du Ihn über ein Netzteil extra mit Strom versorgen!


<sup>*mit Sternchen (\*) gekennzeichnete Links sind Affiliate-/Werbelinks. Wenn du mit diesen einkaufst, dann unterstützt du ProjektionTV! Vielen Dank!*</sup>

## Installation

### Methode 1: Web installer

(tbd)

### Methode 2: Image herunterladen

- Als erstes installiere [esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/) (Benötigt Python und [pip](https://pip.pypa.io/en/stable/installation/))
- Lade die aktuelle Binärdatei von der [Releases](https://github.com/ProjektionTV/ProjektionFX/releases) Seite herunter.  
    Download-Links für die neusten Versionen:
    - [ESP8266 - D1 Mini](https://github.com/realSixi/ProjektionFX/releases/latest/download/ProjektionFX_d1_mini_ESP8266.bin)
    - [ESP8266 - NodeMCU](https://github.com/realSixi/ProjektionFX/releases/latest/download/ProjektionFX_nodemcuv2_ESP8266.bin)
    - [ESP32 - ESP32Thing](https://github.com/realSixi/ProjektionFX/releases/latest/download/ProjektionFX_esp32thing_ESP32.bin)
- Stelle sicher, dass nur *ein* ESP mit deinem Computer verbunden ist, um nicht aus versehen den falschen ESP zu flashen! Du kannst auch direkt den Port angeben, z.B. `-port COM4` nach dem `write_flash` in den nachfolgenden Befehlen

**ESP8266**
```
esptool.py write_flash 0x0 ./ProjektionFX_XXX.bin
```

**ESP32**

Wenn der ESP32 noch keinen Bootloader hat, muss dieser erst geschrieben werden. Lade einen Bootloader, z.B. von dem [WLED Projekt](https://github.com/Aircoookie/WLED/releases/download/v0.13.1/esp32_bootloader_v4.bin) herunter.

```
esptool.py write_flash 0x0 ./esp32_bootloader_v4.bin
```

Anschließend schreibe das ProjektionFX Image. **Achtung:** Achte darauf, dass hier erst ab dem Offset `0x10000` geschrieben wird!
```
esptool.py write_flash 0x10000 ./ProjektionFX_XXX.bin 
```

*Bei manchen ESPs muss der **BOOT** Knopf gedrückt werden, damit ein neues Image geflasht werden kann.*

## Einrichtung

Wenn du ProjektionFX erfolgreich aufgespielt hast, dann öffnet dieser (wenn er an den Strom angeschlossen ist) ein WLAN, über dass du ProjektionFX konfigurieren kannst.

1. Verbinde dich mit dem WLAN **ProjektionFX**, am besten von deinem Handy oder Tablet aus
2. Es öffnet sich eine Seite, um die Konfiguration auszuführen.  
Wenn nicht: öffne in deinem Browser die Adresse [http://192.186.4.1](http://192.186.4.1)
3. Klicke auf **Configure WiFi**:
    1. Wähle **dein WLAN** aus
    2. Trage dein **Passwort** ein
    3. Trage die Daten für den ProjektionTV **MQTT Server** ein.  
    Die *Server-Adresse* sowie Benutzername und Passwort erhälst du entweder im Chat oder du besuchst unseren [Discord-Server](https://discord.gg/92JNBJR)!
4. Tippe auf **Save** um die Einstellungen zu speichern. Der ESP startet nun neu und verbindet sich mit deinem WLAN und empfängt automatisch den Beat von ProjektionTV!    


## Mit-Entwickeln

ProjektionFX ist ein Community Projekt - und wir hoffen, dass viele Menschen Lust und Spaß daran haben, uns bei der Entwicklung zu helfen!

Wenn du eine Idee hast und mit-coden möchtest, dann melde dich am besten gerne im [Discord](https://discord.gg/92JNBJR) oder schreibe ein [Ticket](https://github.com/ProjektionTV/ProjektionFX/issues) im GitHub Repository und beschreibe kurz, was du vor hast.

### Effekte Entwickeln

ProjektionFX wird spannend, wenn möglichst viele Effekte - also Abläufe, wie die LEDs abhängig vom Beat der Musik leuchten - entstehen. 

Wenn du Lust hast deinen eigenen Effekt zu entwickeln, schau dir mal die bestehenden Effekte, z.B. den `effect_movingdot_simple.h` an.


```cpp
class EffectMovingDotSimple : public Effect
{
public:
    static void run(BeatInfo beatInfo, CRGBSet leds, int numLeds)
    {
        // alle LEDs werden etwas "ausgeschaltet", also dunkler
        leds.fadeToBlackBy(100); 

        // ein rotes Licht, dass auf jeden Beat einmal komplett über alle LEDs läuft
        // als erstes wird die Position bestimmt:
        // - die Zeit wo wir uns befinden, wird über die Methode beatInfo.animationFrame(1) abgefragt und 
        //   gibt einen Wert von 0 (= bedeutet, wir sind ganz am Anfang, also der Beat kam gerade) bis 1000 
        //   (das heißt wir sind ganz am Ende, der nächste Beat kommt gleich!) zurück
        // - die map()-Methode 'mappt' (Dreisatz!) die Werte von animationFrame (0-1000) auf den Bereich von
        //   0 bis zu unserer letzten LED (=numLeds)
        // - das Ergebnis wird in der Variable redLEDPosition gespeichert
        int redLEDPosition = map(beatInfo.animationFrame(1), 0, 1000, 0, numLeds);

        // nun schalten wir die LED an der Position redLEDPosition auf rot
        leds[redLEDPosition] = CRGB::Red;

        // ein grünes Licht, dass in der Zeit von zwei Beat einmal komplett über alle LEDs läuft:
        // genauso, wie bei der roten, außer das wir hier beatInfo.animationFrame(2) aufrufen,
        // d.h.:
        // - bei dem Rückgabewert 0 sind wir auf dem 1. Beat,
        // - bei dem Rückgabewert 500 sind wir auf dem 2. Beat
        // - bei dem Wert 1000 sind wir wieder ganz kurz vor dem 1. Beat
        // Insgesamt braucht die grüne LED immer 2 Beats (bei 120BPM wäre das 1 Sekunde), um über den 
        // gesamten LED Streifen zu laufen.
        int greenLEDPosition = map(beatInfo.animationFrame(2), 0, 1000, 0, numLeds);
        leds[greenLEDPosition] = CRGB::Green;


        // zum Abschluss noch ein bisschen Weichzeichnen; ist nicht notwendig, aber etwas 'schöner' und weicher,
        // wenn der Stripe lang ist. Denn auf jeden Beat über viele LEDs laufen ist schon ganz schön schnell ;)
        blur1d(leds, numLeds, 64);
    }
};
```

### Bitte beachten:
- ProjektionFX nutzt die großartige Bibliothek [FastLED](https://github.com/FastLED/FastLED); schau dort gerne in die Dokumentation, welche Funktionen / Hilfsfunktionen zur Verfügung stehen!
- ein Effekt darf/sollte **nicht** die Arduino oder FastLED `delay()` Methode nutzen und damit die Ausführung anhalten.
- die "Animation" der Effekte sollte irgendwie von dem Beat abhängig sein. Da die Lieder meist einen 4/4tel Takt haben, ist es meist am wirkungsvollsten, wenn du auf ein 4-faches von dem Beat 'animierst', also indem du der `beatInfo.animationFrame()`-Methode 4, 8, 16, usw. als Parameter übergibst


### ProjektionFX Kern

Momentan ist ProjektionFX ein kleines Grundgerüst, das noch deutlich weiterentwickelt werden muss. 
Aktuell werden nur die BPM, also die Beats pro Minute, abgefragt und ausgewertet. Aber das soll nicht so bleiben!

- Synchronisierung mit *deinem* Videobild - so dass deine LEDs nicht nur im richtigen Takt, sondern auch zum richtigen Zeitpunkt auf die Musik reagieren. Dies z.B. über
  - eine Photo-Diode und ein optisches Signal im ProjektionTV Videobild
- Synchronisierung, welcher Effekt gerade abgespielt wird
- und vieles mehr :)