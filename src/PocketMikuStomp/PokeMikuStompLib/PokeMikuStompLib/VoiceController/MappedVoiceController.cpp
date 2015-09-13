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
    
    shared_ptr<MikuPhrase> mikuPhrase = make_shared<MikuPhrase>(phrase);
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
    
    return true;
}
