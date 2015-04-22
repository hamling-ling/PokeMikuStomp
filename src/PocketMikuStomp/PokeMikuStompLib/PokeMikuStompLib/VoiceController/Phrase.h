//
//  Phrase.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__Phrase__
#define __PokeMikuStompLib__Phrase__

#include <string>

class Phrase
{
public:
    Phrase();
    Phrase(std::string& phraseString);
    virtual ~Phrase();
    virtual std::string GetPhraseString();
    virtual std::string Next();
    virtual void ResetPos();
    virtual void SetCirculation(bool circulate);
protected:
    std::wstring _phraseString;
    std::wstring::const_iterator _phraseIt;
    bool _circulate;
};


#endif /* defined(__PokeMikuStompLib__Phrase__) */
