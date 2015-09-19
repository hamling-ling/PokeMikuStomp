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
_currentNote(kNoMidiNote)
{
    
}

VoiceController::~VoiceController()
{
    
}

bool VoiceController::SetPhrase(std::string& phrase)
{
    lock_guard<std::recursive_mutex> lock(_phraseMutex);
    
    wstring phraseW = s2ws(phrase);
    _phrase = make_shared<Phrase>(phraseW);
    return true;
}

std::string VoiceController::GetPhrase()
{
    wstring str = _phrase->GetPhraseString();
    return ws2s(str);
}

bool VoiceController::Input(int level, unsigned int note, VoiceControllerNotification& notif)
{
    bool result = false;
    
    if(ShouldStop(level, note, notif)) {
        _currentNote = kNoMidiNote;
        // note off
        notif = MakeFinishedNotification();
        result = true;
        goto FINALY;
    }
    
    if(ShouldStart(level, note, notif)) {
        _currentNote = note;
        notif = MakeStartedNotification();
        result = true;
        goto FINALY;
    }
    
    if(ShouldChange(level, note, notif)) {
        _currentNote = note;
        notif = MakeChangedNotification();
        result = true;
        goto FINALY;
    }
    
FINALY:
    // update level
    _currentInputLevel = level;
    
    return false;
}

VoiceControllerNotification VoiceController::MakeStartedNotification() {
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    return MakeNotification( VoiceControllerNotificationTypePronounceStarted,
                      _currentNote,
                      _phrase->Next());
}

VoiceControllerNotification VoiceController::MakeFinishedNotification() {
    std::wstring pro(L"");
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
                                        const std::wstring& pronounciation
                                        ) {
    _currentPronounciation = pronounciation;
    
    VoiceControllerNotification notif {
        .type = type,
        .pronounciation = ws2s(pronounciation),
        .note = midiNote,
        .level = _currentInputLevel
    };
    
    return notif;
}
