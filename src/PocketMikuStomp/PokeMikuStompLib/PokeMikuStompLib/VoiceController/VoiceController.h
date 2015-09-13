//
//  VoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__VoiceController__
#define __PokeMikuStompLib__VoiceController__

#include <PitchDetector.h>
#include <string>
#include <memory>
#include <functional>
#include <mutex>

typedef enum _VoiceControllerNotificationType {
    VoiceControllerNotificationTypePronounceStarted,
    VoiceControllerNotificationTypePronounceFinished,
    VoiceControllerNotificationTypePronounceChanged
} VoiceControllerNotificationType;

struct VoiceControllerNotification {
    VoiceControllerNotificationType type;
    std::string pronounciation;
    unsigned int note;
    int level;
};

class Phrase;

class VoiceController
{
public:
    static const int kDefaultOffToOnThreshold = 10;
    static const int kDefaultOnToOffThreshold = 5;
    static const unsigned int kNoMidiNote = 0xFFFFFFFF;
    
    VoiceController();
    virtual ~VoiceController();
    virtual bool Input(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool SetPhrase(std::string& phrase);
    virtual std::string GetPhrase();
    virtual void SetThreshold(int offToOn, int onToOff);

protected:
    std::shared_ptr<Phrase> _phrase;
    // last notified note. keep same no matter input level changed.
    unsigned int _currentNote;
    int _currentInputLevel;
    std::wstring _currentPronounciation;
    void* _userInfo;
    int _offToOnThreshold;
    int _onToOffThreshold;
    std::recursive_mutex _phraseMutex;
    
    virtual bool IsBelowOnToOff();
    virtual bool IsAboveOffToOn();
    
    bool HandleInputLevelToOff(int level, VoiceControllerNotification& notif);
    
    virtual VoiceControllerNotification MakeStartedNotification();
    
    virtual VoiceControllerNotification MakeFinishedNotification();
    
    virtual VoiceControllerNotification MakeChangedNotification();

    virtual VoiceControllerNotification MakeNotification(
                                                         VoiceControllerNotificationType type,
                                                         unsigned int midiNote,
                                                         const std::wstring& pronounciation
                                                         );
};

#endif /* defined(__PokeMikuStompLib__VoiceController__) */
