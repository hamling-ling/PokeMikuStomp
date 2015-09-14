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
#include <list>

class Phrase
{
public:
    Phrase();
    Phrase(std::wstring& phraseString);
    virtual ~Phrase();
    virtual std::wstring GetPhraseString();
    virtual std::list<std::wstring> GetPronounciations();
    virtual std::wstring Next();
    virtual void ResetPos();
    virtual void SetCirculation(bool circulate);
    virtual bool GetCirculation();
protected:
    std::wstring _phraseString;
    std::wstring::const_iterator _phraseIt;
    bool _circulate;
};


#endif /* defined(__PokeMikuStompLib__Phrase__) */
