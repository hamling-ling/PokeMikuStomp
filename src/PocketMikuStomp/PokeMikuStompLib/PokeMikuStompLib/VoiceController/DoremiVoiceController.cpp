//
//  DoremiVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/16.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "DoremiVoiceController.h"

using namespace std;

static const string kNoteProStrings[] = {"ど", "ど", "れ", "れ", "み", "ふぁ", "ふぁ", "そ", "そ", "ら", "ら", "し"};

DoremiVoiceController::DoremiVoiceController()
{
    
}

DoremiVoiceController::~DoremiVoiceController()
{
    
}

bool DoremiVoiceController::SetPhrase(std::string& phrase)
{
    return true;
}

std::string DoremiVoiceController::GetPhrase()
{
    return "どれみ";
}

VoiceControllerNotification DoremiVoiceController::MakeStartedNotification() {
    return MakeNotification( VoiceControllerNotificationTypePronounceStarted,
                            _currentNote,
                            kNoteProStrings[_currentNote%12]);
}

VoiceControllerNotification DoremiVoiceController::MakeChangedNotification() {
    return MakeNotification( VoiceControllerNotificationTypePronounceChanged,
                            _currentNote,
                            kNoteProStrings[_currentNote%12]);
}

