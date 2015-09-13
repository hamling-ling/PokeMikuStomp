//
//  StrictVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictVoiceController.h"
#include <memory>
#include "MikuPhrase.h"
#include "Utilities.h"

using namespace std;

StrictVoiceController::StrictVoiceController()
{
}

StrictVoiceController::~StrictVoiceController()
{
}

bool StrictVoiceController::SetPhrase(string& phrase)
{
    // parse phrase. split into transCondisitons and splitPhrase
    string splitPhrase;
    
    return MappedVoiceController::SetPhrase(splitPhrase);
}

bool StrictVoiceController::SplitPhrase(const string& phrase, list<unsigned int>& conds, string& splitPhrase)
{
    wstring phraseW = s2ws(phrase);
    
    std::wstring::const_iterator it = phraseW.begin();
    
    while(it != phraseW.end()) {
        
    }
    return true;
}

bool StrictVoiceController::IsMusicAlpha(const wchar_t letter)
{
    return true;
}

bool StrictVoiceController::IsSharp(const wchar_t legger)
{
    return true;
}

bool StrictVoiceController::IsFlat(const wchar_t legger)
{
    return true;
}

