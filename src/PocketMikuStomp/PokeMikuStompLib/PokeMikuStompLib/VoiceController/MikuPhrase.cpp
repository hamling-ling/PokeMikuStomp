//
//  MikuPhrase.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "MikuPhrase.h"
#include "Utilities.h"
#include "PronouncableLetterMap.h"
#include <iostream>

using namespace std;

MikuPhrase::MikuPhrase()
{
    ResetPos();
}

MikuPhrase::MikuPhrase(std::wstring& phraseString)
:
Phrase(phraseString)
{
    MakePronounciations();
    ResetPos();
}

MikuPhrase::~MikuPhrase()
{
}

wstring MikuPhrase::GetPhraseString()
{
    wstring phrase;
    for (auto item : _pronounciations) {
        phrase += item;
    }
    return phrase;
}

void MikuPhrase::MakePronounciations()
{
    PronouncableLetterMap& map = PronouncableLetterMap::Instance();
    std::wstring::const_iterator it = _phraseString.begin();
    while(it != _phraseString.end()) {
        
        wchar_t letters[3] = {0};
        if(!map.IsPronounsableLetter(*it)) {
            it++;
            continue;
        }
        
        letters[0] = *it;
        it++;
        if(it != _phraseString.end()) {
            if(PronouncableLetterMap::IsSmallVowel(*it)) {
                letters[1] = *it;
                wstring wsCandLetters(letters);
                if(map.Contains(wsCandLetters)) {
                    it++;
                } else {
                    letters[1] = static_cast<wchar_t>(0);
                }
            }
        }
        wstring wsLetters(letters);
        _pronounciations.push_back(letters);
    }
}

void MikuPhrase::SetPronounciatins(const std::list<std::wstring> &pros)
{
    _pronounciations.clear();
    _pronounciations.assign(pros.begin(), pros.end());
    ResetPos();
}

wstring MikuPhrase::Next()
{
    if(_proIt == _pronounciations.end()) {
        if(_circulate) {
        _proIt = _pronounciations.begin();
        } else {
            return L"";
        }
    }

    const wstring& pro = *_proIt;
    _proIt++;
    return pro;
}

void MikuPhrase::ResetPos()
{
    _proIt = _pronounciations.begin();
}
