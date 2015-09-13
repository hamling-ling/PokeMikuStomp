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
    
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    mikuPhrase->SetPronounciatins(splitPhrase);
    
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
    
    return true;
}

