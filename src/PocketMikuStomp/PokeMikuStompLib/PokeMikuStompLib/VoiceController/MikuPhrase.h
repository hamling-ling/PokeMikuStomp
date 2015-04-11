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
#include <vector>

class MikuPhrase : public Phrase
{
public:
    MikuPhrase(std::string& phraseString);
    virtual ~MikuPhrase();
    virtual std::string Next();
    virtual void ResetPos();
private:
    std::vector<std::string> _pronounciations;
    std::vector<std::string>::const_iterator _proIt;
    
    void MakePronounciations();
    bool IsSmallVowel(wchar_t letter);
};

#endif /* defined(__PokeMikuStompLib__MikuPhrase__) */
