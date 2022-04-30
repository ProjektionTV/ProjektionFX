#pragma once

#if !defined(BEAT_INFO_H__)
#define BEAT_INFO_H__

#include <Arduino.h>

#endif // BEAT_INFO_H__

class BeatInfo
{
public:
    void loop();
    virtual unsigned int animationFrame(unsigned int beats);
    double millisPerBeat();
    void changeBPM();
    void setBPM(double bpm);

private:
    unsigned long time;
    double bpm = 120;
};