//
//  StaticVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__StaticVoiceController__
#define __PokeMikuStompLib__StaticVoiceController__

#include "VoiceController.h"

class StaticVoiceController : public VoiceController
{
public:
    StaticVoiceController();
    ~StaticVoiceController();
    virtual void SetPhrase(std::string& phrase);
    virtual bool Input(int level, unsigned int note, VoiceControllerNotification& notif);

private:
    std::string _currentPronounciation;
    
    bool HandleInputLevel(int level, VoiceControllerNotification& notif);
};

#endif /* defined(__PokeMikuStompLib__StaticVoiceController__) */
