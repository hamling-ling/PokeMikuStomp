//
//  StrictPhraseParser.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictPhraseParser.h"
#include <algorithm>

using namespace std;

StrictPhraseParser::StrictPhraseParser()
{
}

StrictPhraseParser::~StrictPhraseParser()
{
}

bool StrictPhraseParser::TrySplit(const std::wstring& phrase,
              std::list<unsigned int>& conds,
              std::string& splitPhrase
              )
{
    std::wstring::const_iterator it = phrase.begin();
    while(it != phrase.end()) {

        it++;
    }
    return true;
}
