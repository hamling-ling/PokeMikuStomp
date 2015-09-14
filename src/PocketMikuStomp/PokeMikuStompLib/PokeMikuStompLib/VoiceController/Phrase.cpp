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

Phrase::Phrase(std::wstring& phraseString)
: _circulate(true)
{
    _phraseString = phraseString;
    ResetPos();
}

Phrase::~Phrase()
{
    
}

wstring Phrase::GetPhraseString()
{
    return _phraseString;
}

list<wstring> Phrase::GetPronounciations()
{
    list<wstring> pros;
    for(auto wc : _phraseString) {
        wchar_t arr[2] = {wc, L'\0'};
        pros.push_back(wstring(arr));
    }

    return pros;
}

wstring Phrase::Next()
{
    if(_phraseIt == _phraseString.end()) {
        if(_circulate) {
            _phraseIt = _phraseString.begin();
        } else {
            return L"";
        }
    }
    
    wchar_t letter = *_phraseIt;
    _phraseIt++;
    wchar_t letters[3] = {letter, (wchar_t)0};
    wstring wsLetter(letters);
    
    return wsLetter;
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

bool Phrase::GetCirculation()
{
    return _circulate;
}
