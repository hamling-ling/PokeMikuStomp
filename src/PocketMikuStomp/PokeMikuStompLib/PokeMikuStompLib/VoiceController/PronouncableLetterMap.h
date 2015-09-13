//
//  PronouncableLetterMap.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__PronouncableLetterMap__
#define __PokeMikuStompLib__PronouncableLetterMap__

#include <map>
#include <string>
#include "Singleton.h"

class PronouncableLetterMap : public Singleton<PronouncableLetterMap>
{
public:
    ~PronouncableLetterMap();
    bool Initialize(const char* path);
    bool IsPronounsableLetter(wchar_t letter);
    bool IsSmallVowel(wchar_t letter);
private:
    std::map<std::wstring, int> _proMap;
    friend class Singleton<PronouncableLetterMap>;
    PronouncableLetterMap();
};

#endif /* defined(__PokeMikuStompLib__PronouncableLetterMap__) */
