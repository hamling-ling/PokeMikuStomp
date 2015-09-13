//
//  StrictVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictVoiceController.h"
#include "MikuPhrase.h"
#include "Utilities.h"
#include "StrictPhraseParser.h"

using namespace std;

StrictVoiceController::StrictVoiceController()
{
}

StrictVoiceController::~StrictVoiceController()
{
}

bool StrictVoiceController::SetPhrase(string& phrase)
{
    wstring phraseW = s2ws(phrase);
    
    StrictPhraseParser parser;
    list<std::wstring> splitPhrase;
    if(!parser.TrySplit(phraseW, _transConditions, splitPhrase)) {
        return false;
    }
    shared_ptr<MikuPhrase> mikuPhrase = make_shared<MikuPhrase>();
    mikuPhrase->SetPronounciatins(splitPhrase);
    
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
    trIt = _transConditions.begin();
    
    return true;
}

bool StrictVoiceController::Input(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(HandleInputLevelToOff(level, notif)) {
        return true;
    }
    
    if(note == kNoMidiNote) {
        return false;
    }
    
    if(!IsAboveOffToOn()) {
        return false;
    }
    
    if(*trIt != kNoMidiNote) {
        if(*trIt == note) {
            return false;
        }
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
