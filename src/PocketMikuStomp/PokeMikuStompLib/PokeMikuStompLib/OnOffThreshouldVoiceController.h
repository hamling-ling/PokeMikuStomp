//
//  OnOffThreshouldVoiceController.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/18.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__OnOffThreshouldVoiceController__
#define __PokeMikuStompLib__OnOffThreshouldVoiceController__

#include "VoiceController.h"

class OnOffThreshouldVoiceController : public VoiceController
{
public:
    static const int kDefaultOffToOnThreshold = 10;
    static const int kDefaultOnToOffThreshold = 5;
    
    OnOffThreshouldVoiceController();
    ~OnOffThreshouldVoiceController();
    void SetThreshold(int offToOn, int onToOff);
protected:
    int _offToOnThreshold;
    int _onToOffThreshold;
    
    virtual bool IsBelowOnToOff(int level);
    virtual bool IsAboveOffToOn(int level);
    virtual bool ShouldStop(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool ShouldStart(int level, unsigned int note, VoiceControllerNotification& notif);
    virtual bool ShouldChange(int level, unsigned int note, VoiceControllerNotification& notif);
};

#endif /* defined(__PokeMikuStompLib__OnOffThreshouldVoiceController__) */
