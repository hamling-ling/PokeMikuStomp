//
//  MappedVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "MappedVoiceController.h"
#include "MikuPhrase.h"
#include "PronouncableLetterMap.h"
#include "Utilities.h"

using namespace std;

MappedVoiceController::MappedVoiceController()
{
}

MappedVoiceController::~MappedVoiceController()
{
}

bool MappedVoiceController::SetPhrase(std::string& phrase)
{
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    
    wstring phraseW = s2ws(phrase);
    shared_ptr<MikuPhrase> mikuPhrase = make_shared<MikuPhrase>(phraseW);
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
    
    return true;
}
