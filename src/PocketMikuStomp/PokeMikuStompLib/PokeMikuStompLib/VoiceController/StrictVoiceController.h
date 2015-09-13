//
//  StrictVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__StrictVoiceController__
#define __PokeMikuStompLib__StrictVoiceController__

#include "MappedVoiceController.h"
#include <list>

class StrictVoiceController : public MappedVoiceController
{
public:
    StrictVoiceController();
    ~StrictVoiceController();
    virtual bool SetPhrase(std::string& phrase);
private:
    std::list<unsigned int> _transConditions;
    
    bool SplitPhrase(const std::string& phrase, std::list<unsigned int>& conds, std::string& splitPhrase);
    bool IsMusicAlpha(const wchar_t letter);
    bool IsSharp(const wchar_t legger);
    bool IsFlat(const wchar_t legger);
};

#endif /* defined(__PokeMikuStompLib__StrictVoiceController__) */
