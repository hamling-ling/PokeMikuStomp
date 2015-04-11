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

VoiceController::VoiceController() :
_threshold(kDefaultThreshold),
_currentNote(kNoMidiNote)
{
    
}

VoiceController::~VoiceController()
{
    
}

void VoiceController::SetPhrase(std::string& phrase)
{
    _phrase = make_shared<Phrase>(phrase);
}

void VoiceController::InputLevel(int level)
{
    _currentInputLevel = level;
}

void VoiceController::InputNote(unsigned int note)
{
    if(_currentNote != note) {
        _currentNote = note;
    }
}

void VoiceController::SetCallback(VoiceControllerCallback_t func, void* userInfo)
{
    _callback = func;
    _userInfo = userInfo;
}

void VoiceController::SetThreshold(int threshold)
{
    _threshold = threshold;
}

bool VoiceController::IsOffLevel() {
    return (_currentInputLevel < _threshold);
}

void VoiceController::RaisePronounceStartedNotification() {
    RaiseNotification( VoiceControllerNotificationTypePronounceStarted,
                      _currentNote,
                      _phrase->Next());
}

void VoiceController::RaisePronounceFinishedNotification() {
    std::string pro("");
    RaiseNotification( VoiceControllerNotificationTypePronounceFinished, kNoMidiNote, pro);
}

void VoiceController::RaisePronounceChangedNotification() {
    RaiseNotification( VoiceControllerNotificationTypePronounceChanged,
                      _currentNote,
                      _phrase->Next());
}

void VoiceController::RaiseNotification(
                                        VoiceControllerNotificationType type,
                                        unsigned int midiNote,
                                        const std::string& pronounciation
                                        ) {
    VoiceControllerNotification notif {
        .type = type,
        .pronounciation = pronounciation,
        .note = midiNote,
        .user = _userInfo,
        .level = _currentInputLevel
    };
    
    if(_callback) {
        _callback(notif);
    }
}
