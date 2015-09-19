//
//  OnOffThreshouldVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/18.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "OnOffThreshouldVoiceController.h"
#include <cassert>

using namespace std;

OnOffThreshouldVoiceController::OnOffThreshouldVoiceController() :
_offToOnThreshold(kDefaultOffToOnThreshold),
_onToOffThreshold(kDefaultOffToOnThreshold)
{
    
}

OnOffThreshouldVoiceController::~OnOffThreshouldVoiceController()
{
    
}

void OnOffThreshouldVoiceController::SetThreshold(int offToOn, int onToOff)
{
    _offToOnThreshold = offToOn;
    _onToOffThreshold = onToOff;
}

bool OnOffThreshouldVoiceController::IsBelowOnToOff(int level) {
    return (level < _onToOffThreshold);
}

bool OnOffThreshouldVoiceController::IsAboveOffToOn(int level) {
    return (_offToOnThreshold <= level);
}

bool OnOffThreshouldVoiceController::ShouldStop(int level, unsigned int note, VoiceControllerNotification& notif)
{
    bool isOn = (kNoMidiNote != _currentNote);
    
    if(!isOn) {
        return false;
    }
    
    if( isOn && IsBelowOnToOff(level)) {
        return true;
    }
    
    return false;
}

bool OnOffThreshouldVoiceController::ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(note == kNoMidiNote) {
        return false;
    }
    
    if(!IsAboveOffToOn(level)) {
        return false;
    }
    
    if(_currentNote == kNoMidiNote) {
        return true;
    }
    return false;
}

bool OnOffThreshouldVoiceController::ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(note == kNoMidiNote) {
        return false;
    }
    
    if(!IsAboveOffToOn(level)) {
        return false;
    }
    
    if( IsBelowOnToOff(level)) {
        return false;
    }
    
    return true;
}
