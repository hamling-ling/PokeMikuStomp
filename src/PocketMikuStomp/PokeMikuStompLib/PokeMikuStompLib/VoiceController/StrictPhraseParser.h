//
//  StrictPhraseParser.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__StrictPhraseParser__
#define __PokeMikuStompLib__StrictPhraseParser__

#include <list>
#include <map>
#include <string>

class StrictPhraseParser
{
public:
    StrictPhraseParser();
    ~StrictPhraseParser();
    bool TrySplit(const std::wstring& phrase,
                  std::list<unsigned int>& conds,
                  std::string& splitPhrase
                  );
};

#endif /* defined(__PokeMikuStompLib__StrictPhraseParser__) */
