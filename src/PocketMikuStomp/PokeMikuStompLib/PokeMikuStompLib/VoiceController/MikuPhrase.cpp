//
//  MikuPhrase.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "MikuPhrase.h"
#include "Utilities.h"

using namespace std;

MikuPhrase::MikuPhrase(std::string& phraseString)
:
Phrase(phraseString)
{
    MakePronounciations();
    ResetPos();
}

MikuPhrase::~MikuPhrase()
{
    
}

void MikuPhrase::MakePronounciations()
{
    std::wstring::const_iterator it = _phraseString.begin();
    while(it != _phraseString.end()) {
        wchar_t letters[3] = {0};
        letters[0] = *it;
        it++;
        if(it != _phraseString.end()) {
            if(IsSmallVowel(*it)) {
                letters[1] = *it;
            }
        }
        wstring wsLetters(letters);
        string sLetters = ws2s(wsLetters);
        _pronounciations.push_back(sLetters);
    }
}

string MikuPhrase::Next()
{
    if(_proIt == _pronounciations.end()) {
        if(_circulate) {
        _proIt = _pronounciations.begin();
        } else {
            return "";
        }
    }

    const string& pro = *_proIt;
    _proIt++;
    return pro;
}

void MikuPhrase::ResetPos()
{
    _proIt = _pronounciations.begin();
}

bool MikuPhrase::IsSmallVowel(wchar_t letter)
{
    bool isSmallVowel = false;
    switch (letter) {
        case L'ぁ':
        case L'ぃ':
        case L'ぅ':
        case L'ぇ':
        case L'ぉ':
        case L'ゃ':
        case L'ゅ':
        case L'ょ':
            isSmallVowel = true;
            break;
        default:
            break;
    }
    return isSmallVowel;
}
