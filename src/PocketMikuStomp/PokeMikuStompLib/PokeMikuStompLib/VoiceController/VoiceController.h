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
    void* user;
};

class Phrase;

typedef std::function<void(const VoiceControllerNotification&)> VoiceControllerCallback_t;

class VoiceController
{
public:
    static const int kDefaultThreshold = 20;;
    static const unsigned int kNoMidiNote = 0xFFFFFFFF;
    
    VoiceController();
    virtual ~VoiceController();
    virtual void InputLevel(int level);
    virtual void InputNote(unsigned int note);
    virtual void SetPhrase(std::string& phrase);
    virtual void SetCallback(VoiceControllerCallback_t func, void* userInfo);
    virtual void SetThreshold(int threshold);

protected:
    std::shared_ptr<Phrase> _phrase;
    VoiceControllerCallback_t _callback;
    // last notified note. keep same no matter input level changed.
    unsigned int _currentNote;
    int _currentInputLevel;
    void* _userInfo;
    int _threshold;
    
    virtual bool IsOffLevel();
    
    virtual void RaisePronounceStartedNotification();
    
    virtual void RaisePronounceFinishedNotification();
    
    virtual void RaisePronounceChangedNotification();

private:
    void RaiseNotification(
                           VoiceControllerNotificationType type,
                           unsigned int midiNote,
                           const std::string& pronounciation
                           );
};

#endif /* defined(__PokeMikuStompLib__VoiceController__) */
