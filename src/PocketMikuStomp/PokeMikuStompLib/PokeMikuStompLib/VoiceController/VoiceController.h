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

class Phrase;

class VoiceController
{
public:
    VoiceController();
    virtual ~VoiceController();
    virtual void Input(PitchInfo& pitch);
    virtual void SetPhrase(std::string& phrase);

protected:
    std::shared_ptr<Phrase> _phrase;
    
    virtual wchar_t ReadLetter();
};

#endif /* defined(__PokeMikuStompLib__VoiceController__) */
