//
//  VoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "VoiceController.h"
#include "Utilities.h"
#include "Phrase.h"

using namespace std;

VoiceController::VoiceController()
{
    
}

VoiceController::~VoiceController()
{
    
}

void VoiceController::SetPhrase(std::string& phrase)
{
    _phrase = make_shared<Phrase>(phrase);
}

void VoiceController::Input(PitchInfo& pitch)
{
    
}

