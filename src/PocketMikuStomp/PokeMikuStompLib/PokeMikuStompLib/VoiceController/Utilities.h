//
//  Utilities.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__Utilities__
#define __PokeMikuStompLib__Utilities__


#include <string>
#include <codecvt>
#include <locale>

using namespace std;

wstring s2ws(const std::string& str);

string ws2s(const std::wstring& wstr);

#endif /* defined(__PokeMikuStompLib__Utilities__) */
