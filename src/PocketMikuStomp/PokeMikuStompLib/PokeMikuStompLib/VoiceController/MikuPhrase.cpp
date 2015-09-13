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

std::string MikuPhrase::GetPhraseString()
{
    string phrase;
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
