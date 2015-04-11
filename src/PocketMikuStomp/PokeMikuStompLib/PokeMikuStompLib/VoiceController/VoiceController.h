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
    static const int kDefaultThreshold = 20;;
    static const unsigned int kNoMidiNote = 0xFFFFFFFF;
    
    VoiceController();
    virtual ~VoiceController();
    virtual bool Input(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual void SetPhrase(std::string& phrase);
    virtual void SetThreshold(int threshold);

protected:
    std::shared_ptr<Phrase> _phrase;
    // last notified note. keep same no matter input level changed.
    unsigned int _currentNote;
    int _currentInputLevel;
    void* _userInfo;
    int _threshold;
    
    virtual bool IsOffLevel();
    
    virtual VoiceControllerNotification MakeStartedNotification();
    
    virtual VoiceControllerNotification MakeFinishedNotification();
    
    virtual VoiceControllerNotification MakeChangedNotification();

private:
    VoiceControllerNotification MakeNotification(
                                             VoiceControllerNotificationType type,
                                             unsigned int midiNote,
                                             const std::string& pronounciation
                                             );
};

#endif /* defined(__PokeMikuStompLib__VoiceController__) */
