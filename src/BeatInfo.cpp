
#include "BeatInfo.h"

/**
 * @brief maps the provided number of beats to a range from 0 to 999 according to the current bpm
 *
 * @param beats the number of beats
 * @return unsigned int the current animation frame (0 - 999)
 */
unsigned int BeatInfo::animationFrame(unsigned int beats)
{
    double millisPerBeat = this->millisPerBeat();
    unsigned long divisor = millisPerBeat * beats;
    unsigned int result = map(this->time % divisor, 0, divisor, 0, 1000);

    return result;
}

/**
 * @brief must be called in the main loop, updates the current time
 *
 */
void BeatInfo::loop()
{
    this->time = millis();
}

/**
 * @brief returns the interval between two beats in milliseconds
 *
 * @return double interval in milliseconds
 */
double BeatInfo::millisPerBeat()
{
    double millisPerBeat = 60000. / this->bpm;
    return millisPerBeat;
}


void BeatInfo::setBPM(double bpm){
    this->bpm = bpm;
    Serial.printf("BPM set to: %f", bpm);
}


void BeatInfo::changeBPM()
{
    // switch (random(3))
    // {
    // case 0:
    //     this->bpm = 120;
    //     break;
    // case 1:
    //     this->bpm = 180;
    //     break;
    // case 3:
    //     this->bpm = 140;
    //     break;
    // }

    Serial.printf("BPM Changed to %f\n", this->bpm);
}