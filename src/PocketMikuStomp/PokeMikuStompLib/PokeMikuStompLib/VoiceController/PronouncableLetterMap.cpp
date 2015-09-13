//
//  PronouncableLetterMap.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "PronouncableLetterMap.h"
#include <fstream>
#include "Utilities.h"

using namespace std;

PronouncableLetterMap::PronouncableLetterMap()
{
}

PronouncableLetterMap::~PronouncableLetterMap()
{
}

bool PronouncableLetterMap::Initialize(const char* path) {
    ifstream file(path);
    if(!file.is_open()) {
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        wstring wline = s2ws(line);
        pair<wstring, int> item(wline, count++);
        _proMap.insert(item);
    }
    file.close();
    
    return true;
}

bool PronouncableLetterMap::IsPronounsableLetter(wchar_t letter)
{
    wchar_t letters[2] = {letter, 0};
    wstring wsLetter(letters);
    return (_proMap.find(wsLetter) != _proMap.end());
}

bool PronouncableLetterMap::IsSmallVowel(wchar_t letter)
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

