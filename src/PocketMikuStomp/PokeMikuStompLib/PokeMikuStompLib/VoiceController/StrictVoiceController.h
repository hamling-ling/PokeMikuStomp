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
    virtual std::string GetPhrase();
private:
    std::list<unsigned int> _transConditions;
    std::list<unsigned int>::const_iterator trIt;
    
    virtual bool ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif);
    void LoadNextConditon();
};

#endif /* defined(__PokeMikuStompLib__StrictVoiceController__) */
