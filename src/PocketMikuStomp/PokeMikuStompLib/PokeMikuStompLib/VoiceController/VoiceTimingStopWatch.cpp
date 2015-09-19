//
//  VoiceTimingStopWatch.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/18.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "VoiceTimingStopWatch.h"

using namespace std;
using namespace std::chrono;

VoiceTimingStopWatch::VoiceTimingStopWatch() :
_startTime(system_clock::now()),
_isRunning(false),
_interval(0)
{
}

VoiceTimingStopWatch::~VoiceTimingStopWatch()
{
}

unsigned long long VoiceTimingStopWatch::Time()
{
    lock_guard<std::recursive_mutex> lock(_mutex);
    
    if(!_isRunning) {
        return 0;
    }
    
    auto endTime = system_clock::now();
    typedef duration<unsigned long long, microseconds::period> fp_microsec;
    unsigned long long time = fp_microsec(endTime - _startTime).count();
    
    return time;
}

void VoiceTimingStopWatch::Reset(unsigned int interval)
{
    lock_guard<std::recursive_mutex> lock(_mutex);
    
    _startTime = system_clock::now();
    _interval = interval;
    _isRunning = true;
}

bool VoiceTimingStopWatch::IsEllapsed()
{
    lock_guard<std::recursive_mutex> lock(_mutex);
    
    unsigned long long time = Time();
    if(_interval < time) {
        return true;
    }
    return false;
}
