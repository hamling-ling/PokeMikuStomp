//
//  MikuPhrase.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__MikuPhrase__
#define __PokeMikuStompLib__MikuPhrase__

#include "Phrase.h"
#include <list>
#include <map>

class MikuPhrase : public Phrase
{
public:
    MikuPhrase();
    MikuPhrase(std::wstring& phraseString);
    virtual ~MikuPhrase();
    virtual std::wstring GetPhraseString();
    virtual std::list<std::wstring> GetPronounciations();
    virtual std::wstring Next();
    virtual void ResetPos();
    virtual void SetPronounciatins(const std::list<std::wstring> &pros);
private:
    std::list<std::wstring> _pronounciations;
    std::list<std::wstring>::const_iterator _proIt;
    
    void MakePronounciations();
};

#endif /* defined(__PokeMikuStompLib__MikuPhrase__) */
