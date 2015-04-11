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
    virtual void Input(PitchInfo& pitch);
};

#endif /* defined(__PokeMikuStompLib__StaticVoiceController__) */
