//
//  VoiceTimingStopWatch.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/18.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__VoiceTimingStopWatch__
#define __PokeMikuStompLib__VoiceTimingStopWatch__

#include <chrono>
#include <mutex>

class VoiceTimingStopWatch
{
public:
    VoiceTimingStopWatch();
    ~VoiceTimingStopWatch();
    unsigned long long Time();
    void Reset(unsigned int);
    bool IsEllapsed();
private:
    std::chrono::time_point<std::chrono::system_clock> _startTime;
    std::recursive_mutex _mutex;
    bool _isRunning;
    unsigned int _interval;
};

#endif /* defined(__PokeMikuStompLib__VoiceTimingStopWatch__) */
