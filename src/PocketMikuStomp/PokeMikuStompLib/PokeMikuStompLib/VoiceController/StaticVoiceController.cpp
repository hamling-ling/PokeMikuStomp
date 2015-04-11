//
//  StaticVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StaticVoiceController.h"

StaticVoiceController::StaticVoiceController()
{
    
}

StaticVoiceController::~StaticVoiceController()
{
    
}

void StaticVoiceController::InputLevel(int level)
{
    bool isOffLevel = IsOffLevel();
    bool willBeOffLevel = (level < _threshold);

    // update level
    _currentInputLevel = level;
    
    if(!isOffLevel && willBeOffLevel) {
        _currentPronounciation = "";
        _currentNote = kNoMidiNote;
        
        // note off
        RaisePronounceFinishedNotification();
    }
}

void StaticVoiceController::InputNote(unsigned int note)
{
    if(IsOffLevel()) {
        return;
    }
    
    if(_currentNote == kNoMidiNote) {
        _currentNote = note;
        RaisePronounceStartedNotification();
        return;
    }
    
    if(_currentNote != note) {
        _currentNote = note;
        RaisePronounceChangedNotification();
    }
}
