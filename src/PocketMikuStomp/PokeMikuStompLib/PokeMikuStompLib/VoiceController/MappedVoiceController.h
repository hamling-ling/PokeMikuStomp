//
//  MappedVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__MappedVoiceController__
#define __PokeMikuStompLib__MappedVoiceController__

#include "OnOffThreshouldVoiceController.h"
#include "PronouncableLetterMap.h"

class MappedVoiceController : public OnOffThreshouldVoiceController
{
public:
    MappedVoiceController();
    ~MappedVoiceController();
    virtual bool SetPhrase(std::string& phrase);
};

#endif /* defined(__PokeMikuStompLib__MappedVoiceController__) */
