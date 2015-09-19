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
    bool isOn = (kNoMidiNote != _currentNote);
    
    if(isOn) {
        if(ShouldStop(level, note, notif)) {
            notif = StopPronounce(level);
            result = true;
            goto FINALY;
        }
        if(ShouldChange(level, note, notif)) {
            notif = ChangePronounce(level, note);
            result = true;
            goto FINALY;
        }
    } else {
        if(ShouldStart(level, note, notif)) {
            notif = StartedPronounce(level, note);
            result = true;
            goto FINALY;
        }
    }

FINALY:
    // update level
    _currentInputLevel = level;
    
    return result;
}

VoiceControllerNotification VoiceController::StartedPronounce(int level, unsigned int note) {
    const std::wstring& pro = GetNextPronounciation(level, note);
    UpdateInternalState(level, note, pro);
    
    return MakeNotification(VoiceControllerNotificationTypePronounceStarted,
                            level,
                            note,
                            pro);
}

VoiceControllerNotification VoiceController::StopPronounce(int level) {
    const std::wstring pro(L"");
    UpdateInternalState(level, kNoMidiNote, pro);
    return MakeNotification( VoiceControllerNotificationTypePronounceFinished, level, kNoMidiNote, pro);
}

VoiceControllerNotification VoiceController::ChangePronounce(int level, unsigned int note) {
    const std::wstring& pro = GetNextPronounciation(level, note);
    UpdateInternalState(level, note, pro);
    
    return MakeNotification(VoiceControllerNotificationTypePronounceChanged,
                            level,
                            note,
                            pro);
}

void VoiceController::UpdateInternalState(int level, unsigned int note, const wstring& pro)
{
    _currentInputLevel = level;
    _currentNote = note;
    _currentPronounciation = pro;
}

VoiceControllerNotification VoiceController::MakeNotification (
                                        VoiceControllerNotificationType type,
                                        int level,
                                        unsigned int midiNote,
                                        const std::wstring& pronounciation
                                        ) {
    VoiceControllerNotification notif {
        .type = type,
        .pronounciation = ws2s(pronounciation),
        .note = midiNote,
        .level = level
    };
    
    return notif;
}

wstring VoiceController::GetNextPronounciation(int level, unsigned int note)
{
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    return _phrase->Next();
}
