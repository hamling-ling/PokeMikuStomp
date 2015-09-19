//
//  AvoidTooShortVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/18.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__AvoidTooShortVoiceController__
#define __PokeMikuStompLib__AvoidTooShortVoiceController__

#include "StrictVoiceController.h"
#include "VoiceTimingStopWatch.h"

class AvoidTooShortVoiceController : public StrictVoiceController
{
public:
    AvoidTooShortVoiceController();
    ~AvoidTooShortVoiceController();

    virtual void SetMinimumVoiceLength(unsigned int len);
protected:
    unsigned int _minVoiceLen;
    VoiceTimingStopWatch _stopWatch;
    
    virtual bool ShouldStop(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif);
};

#endif /* defined(__PokeMikuStompLib__AvoidTooShortVoiceController__) */
