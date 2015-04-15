//
//  DoremiVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/16.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__DoremiVoiceController__
#define __PokeMikuStompLib__DoremiVoiceController__

#include "VoiceController.h"

class DoremiVoiceController : public VoiceController
{
public:
    DoremiVoiceController();
    ~DoremiVoiceController();
    
    virtual bool SetPhrase(std::string& phrase);
    virtual std::string GetPhrase();
    
protected:
    
    virtual VoiceControllerNotification MakeStartedNotification();
    
    virtual VoiceControllerNotification MakeChangedNotification();
};

#endif /* defined(__PokeMikuStompLib__DoremiVoiceController__) */
