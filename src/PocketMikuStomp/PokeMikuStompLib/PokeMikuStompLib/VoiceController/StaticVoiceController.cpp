//
//  StaticVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StaticVoiceController.h"
#include "MikuPhrase.h"

using namespace std;

StaticVoiceController::StaticVoiceController()
{
    
}

StaticVoiceController::~StaticVoiceController()
{
    
}

void StaticVoiceController::SetPhrase(std::string& phrase)
{
    shared_ptr<MikuPhrase> mikuPhrase = make_shared<MikuPhrase>(phrase);
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
}

bool StaticVoiceController::HandleInputLevel(int level, VoiceControllerNotification& notif)
{
    bool isOffLevel = IsOffLevel();
    bool willBeOffLevel = (level < _threshold);

    // update level
    _currentInputLevel = level;
    
    if(!isOffLevel && willBeOffLevel) {
        _currentPronounciation = "";
        _currentNote = kNoMidiNote;
        
        // note off
        notif = MakeFinishedNotification();
        return true;
    }
    
    return false;
}

bool StaticVoiceController::Input(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(HandleInputLevel(level, notif)) {
        return true;
    }
    
    if(note == kNoMidiNote) {
        return false;
    }
    
    if(level < _threshold) {
        return false;
    }
    
    if(_currentNote == kNoMidiNote) {
        _currentNote = note;
        notif = MakeStartedNotification();
        return true;
    }
    
    if(_currentNote != note) {
        _currentNote = note;
        notif = MakeChangedNotification();
        return true;
    }
    
    return false;
}
