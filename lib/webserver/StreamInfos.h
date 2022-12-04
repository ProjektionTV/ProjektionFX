#ifndef __STREAM_INFOS__
#define __STREAM_INFOS__

#include <Arduino.h>

class StreamInfos {
  public:
    uint64_t latency = 0;
};

extern StreamInfos streamInfos;

#endif
