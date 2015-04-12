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
#include <map>

class StaticVoiceController : public VoiceController
{
public:
    StaticVoiceController();
    ~StaticVoiceController();
    static bool MakePronounceStringMap(const char* path, std::map<std::wstring, int>& map);
    virtual bool SetPhrase(std::string& phrase, const char* mapPath);
    virtual bool Input(int level, unsigned int note, VoiceControllerNotification& notif);
    
private:
    std::string _currentPronounciation;
    std::map<std::wstring, int> _proMap;
    
    bool MakeMap(const char* mapPath);
    bool HandleInputLevel(int level, VoiceControllerNotification& notif);
};

#endif /* defined(__PokeMikuStompLib__StaticVoiceController__) */
