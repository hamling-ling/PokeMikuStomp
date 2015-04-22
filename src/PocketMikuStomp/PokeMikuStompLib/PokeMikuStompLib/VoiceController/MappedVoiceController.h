//
//  MappedVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__MappedVoiceController__
#define __PokeMikuStompLib__MappedVoiceController__

#include "VoiceController.h"
#include <map>

class MappedVoiceController : public VoiceController
{
public:
    MappedVoiceController();
    ~MappedVoiceController();
    static bool MakePronounceStringMap(const char* path, std::map<std::wstring, int>& map);
    virtual bool SetMap(const char* mapPath);
    virtual bool SetPhrase(std::string& phrase);
    
private:
    std::map<std::wstring, int> _proMap;
    
    bool MakeMap(const char* mapPath);
};

#endif /* defined(__PokeMikuStompLib__MappedVoiceController__) */
