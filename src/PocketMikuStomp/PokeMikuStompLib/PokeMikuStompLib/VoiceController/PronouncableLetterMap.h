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
#include <mutex>
#include "Singleton.h"

class PronouncableLetterMap : public Singleton<PronouncableLetterMap>
{
public:
    ~PronouncableLetterMap();
    bool Initialize(const char* path);
    bool IsInitialized();
    bool IsPronounsableLetter(wchar_t letter);
    bool IsPronounsableLetter(const std::wstring& pron);
    static bool IsSmallVowel(wchar_t letter);
    static bool IsSmallVowel(const std::wstring& pron);
    bool Contains(const std::wstring& str);
private:
    std::mutex _mapMutex;
    std::map<std::wstring, int> _proMap;
    bool _initialized;
    friend class Singleton<PronouncableLetterMap>;
    PronouncableLetterMap();
};

#endif /* defined(__PokeMikuStompLib__PronouncableLetterMap__) */
