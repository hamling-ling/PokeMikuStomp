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
    if(parser.TrySplit(phraseW, _transConditions, splitPhrase)) {
        
    }
    
    // t.b.d
    string str;
    return MappedVoiceController::SetPhrase(str);
}

