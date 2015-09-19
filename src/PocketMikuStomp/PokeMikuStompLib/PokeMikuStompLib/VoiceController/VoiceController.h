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
#include "VoiceControllerCommon.h"

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
    VoiceController();
    virtual ~VoiceController();
    virtual bool Input(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool SetPhrase(std::string& phrase);
    virtual std::string GetPhrase();

protected:
    std::shared_ptr<Phrase> _phrase;
    // last notified note. keep same no matter input level changed.
    unsigned int _currentNote;
    int _currentInputLevel;
    std::wstring _currentPronounciation;
    void* _userInfo;
    std::recursive_mutex _phraseMutex;
    
    // check if voice should stop. called before ShouldStart
    virtual bool ShouldStop(int level, unsigned int note, VoiceControllerNotification& notif) = 0;
    // check if voice should start. called after ShouldStop
    virtual bool ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif) = 0;
    // check if voice should change. called after ShouldStart
    virtual bool ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif) = 0;
    virtual VoiceControllerNotification StartedPronounce(int level, unsigned int note);
    virtual VoiceControllerNotification StopPronounce(int level);
    virtual VoiceControllerNotification ChangePronounce(int level, unsigned int note);
    virtual void UpdateInternalState(int level, unsigned int note, const std::wstring& pro);
    virtual VoiceControllerNotification MakeNotification(
                                                         VoiceControllerNotificationType type,
                                                         int level,
                                                         unsigned int midiNote,
                                                         const std::wstring& pronounciation
                                                         );
    virtual std::wstring GetNextPronounciation(int level, unsigned int note);
};

#endif /* defined(__PokeMikuStompLib__VoiceController__) */
