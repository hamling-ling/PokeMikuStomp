//
//  AvoidTooShortVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/18.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "AvoidTooShortVoiceController.h"
#include <cassert>

using namespace std;

AvoidTooShortVoiceController::AvoidTooShortVoiceController() :
_minVoiceLen(100)
{
}

AvoidTooShortVoiceController::~AvoidTooShortVoiceController()
{
}

void AvoidTooShortVoiceController::SetMinimumVoiceLength(unsigned int len)
{
    _minVoiceLen = len;
}

bool AvoidTooShortVoiceController::ShouldStop(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(!_stopWatch.IsEllapsed()) {
        return false;
    }
    
    bool result = OnOffThreshouldVoiceController::ShouldStop(level, note, notif);
    if(result) {
        // anything to do?
    }
    return result;
}

bool AvoidTooShortVoiceController::ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif)
{
    bool result = OnOffThreshouldVoiceController::ShouldStart(level, note, notif);
    if(result) {
        _stopWatch.Reset(_minVoiceLen);
    }
    return result;
}

bool AvoidTooShortVoiceController::ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(!_stopWatch.IsEllapsed()) {
        return false;
    }
    
    bool result = OnOffThreshouldVoiceController::ShouldChange(level, note, notif);
    if(result) {
        // anything to do?
        _stopWatch.Reset(_minVoiceLen);
    }
    return result;
}
