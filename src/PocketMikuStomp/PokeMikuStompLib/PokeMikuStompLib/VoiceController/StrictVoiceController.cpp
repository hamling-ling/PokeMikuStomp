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

std::string StrictVoiceController::GetPhrase()
{
    wstring phrase;
    list<wstring> pros = _phrase->GetPronounciations();
    list<wstring>::const_iterator proIt = pros.begin();
    list<unsigned int>::const_iterator cndIt = _transConditions.begin();
    
    static const wstring noteTable[] = {L"C",L"C#",L"D",L"D#",L"E",L"F",L"F#",L"G",L"G#",L"A",L"B♭",L"B"};
    while (proIt != pros.end() && cndIt != _transConditions.end()) {
        wstring note;
        if(*cndIt != kNoMidiNote) {
            note = noteTable[*cndIt];
        }
        phrase += note + *proIt;
        
        proIt++;
        cndIt++;
    }
    
    return ws2s(phrase);
}

bool StrictVoiceController::ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(*trIt != kNoMidiNote) {
        if(*trIt != note%12) {
            return false;
        }
    }
    
    bool result = OnOffThreshouldVoiceController::ShouldStart(level, note, notif);
    if(result) {
        LoadNextConditon();
    }
    return result;
}

bool StrictVoiceController::ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(*trIt != kNoMidiNote) {
        if(*trIt != note%12) {
            return false;
        }
    }
    
    bool result = OnOffThreshouldVoiceController::ShouldChange(level, note, notif);
    if(result) {
        LoadNextConditon();
    }
    return result;
}

void StrictVoiceController::LoadNextConditon()
{
    trIt++;
    if(trIt == _transConditions.end()) {
        if(_phrase->GetCirculation()) {
            trIt = _transConditions.begin();
        }
    }
}
