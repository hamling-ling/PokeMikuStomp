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
_offToOnThreshold(kDefaultOffToOnThreshold),
_onToOffThreshold(kDefaultOffToOnThreshold),
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

bool VoiceController::Input(int level, unsigned int note, VoiceControllerNotification& notif)
{
    _currentInputLevel = level;
    _currentNote = note;
    return true;
}

void VoiceController::SetThreshold(int offToOn, int onToOff)
{
    _offToOnThreshold = offToOn;
    _onToOffThreshold = onToOff;
}

bool VoiceController::IsBelowOnToOff() {
    return (_currentInputLevel < _onToOffThreshold);
}

bool VoiceController::IsAboveOffToOn() {
    return (_offToOnThreshold <= _currentInputLevel);
}

VoiceControllerNotification VoiceController::MakeStartedNotification() {
    return MakeNotification( VoiceControllerNotificationTypePronounceStarted,
                      _currentNote,
                      _phrase->Next());
}

VoiceControllerNotification VoiceController::MakeFinishedNotification() {
    std::string pro("");
    return MakeNotification( VoiceControllerNotificationTypePronounceFinished, kNoMidiNote, pro);
}

VoiceControllerNotification VoiceController::MakeChangedNotification() {
    return MakeNotification( VoiceControllerNotificationTypePronounceChanged,
                      _currentNote,
                      _phrase->Next());
}

VoiceControllerNotification VoiceController::MakeNotification (
                                        VoiceControllerNotificationType type,
                                        unsigned int midiNote,
                                        const std::string& pronounciation
                                        ) {
    VoiceControllerNotification notif {
        .type = type,
        .pronounciation = pronounciation,
        .note = midiNote,
        .level = _currentInputLevel
    };
    
    return notif;
}
