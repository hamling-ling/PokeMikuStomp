//
//  Phrase.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "Phrase.h"
#include "Utilities.h"

Phrase::Phrase()
: _circulate(true)
{
    ResetPos();
}

Phrase::Phrase(std::string& phraseString)
: _circulate(true)
{
    _phraseString = s2ws(phraseString);
    ResetPos();
}

Phrase::~Phrase()
{
    
}

string Phrase::Next()
{
    if(_phraseIt == _phraseString.end()) {
        if(_circulate) {
            _phraseIt = _phraseString.begin();
        } else {
            return "";
        }
    }
    
    wchar_t letter = *_phraseIt;
    _phraseIt++;
    wchar_t letters[3] = {letter, (wchar_t)0};
    wstring wsLetter(letters);
    
    return ws2s(wsLetter);
}

void Phrase::ResetPos()
{
    _phraseIt = _phraseString.begin();
}

void Phrase::SetCirculation(bool circulate)
{
    _circulate = circulate;
    ResetPos();
}
