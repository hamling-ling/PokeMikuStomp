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

bool VoiceController::SetPhrase(std::string& phrase)
{
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    _phrase = make_shared<Phrase>(phrase);
    return true;
}

std::string VoiceController::GetPhrase()
{
    return _phrase->GetPhraseString();
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

bool VoiceController::HandleInputLevelToOff(int level, VoiceControllerNotification& notif)
{
    bool isOn = (kNoMidiNote != _currentNote);
    // update level
    _currentInputLevel = level;
    
    if( isOn && IsBelowOnToOff()) {
        _currentNote = kNoMidiNote;
        // note off
        notif = MakeFinishedNotification();
        return true;
    }
    
    return false;
}

bool VoiceController::Input(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(HandleInputLevelToOff(level, notif)) {
        return true;
    }
    
    if(note == kNoMidiNote) {
        return false;
    }
    
    if(!IsAboveOffToOn()) {
        return false;
    }
    
    if(_currentNote == kNoMidiNote) {
        _currentNote = note;
        notif = MakeStartedNotification();
        return true;
    }
    
    if(_currentNote != note) {
        _currentNote = note;
        notif = MakeChangedNotification();
        return true;
    }
    
    return false;
}

VoiceControllerNotification VoiceController::MakeStartedNotification() {
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    return MakeNotification( VoiceControllerNotificationTypePronounceStarted,
                      _currentNote,
                      _phrase->Next());
}

VoiceControllerNotification VoiceController::MakeFinishedNotification() {
    std::string pro("");
    return MakeNotification( VoiceControllerNotificationTypePronounceFinished, kNoMidiNote, pro);
}

VoiceControllerNotification VoiceController::MakeChangedNotification() {
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    return MakeNotification( VoiceControllerNotificationTypePronounceChanged,
                      _currentNote,
                      _phrase->Next());
}

VoiceControllerNotification VoiceController::MakeNotification (
                                        VoiceControllerNotificationType type,
                                        unsigned int midiNote,
                                        const std::string& pronounciation
                                        ) {
    _currentPronounciation = pronounciation;
    
    VoiceControllerNotification notif {
        .type = type,
        .pronounciation = pronounciation,
        .note = midiNote,
        .level = _currentInputLevel
    };
    
    return notif;
}
