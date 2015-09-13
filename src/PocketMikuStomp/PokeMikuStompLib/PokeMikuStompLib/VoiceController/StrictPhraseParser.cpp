//
//  StrictPhraseParser.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictPhraseParser.h"
#include "StrictPhraseStateMachine.h"

using namespace std;

StrictPhraseParser::StrictPhraseParser()
{
}

StrictPhraseParser::~StrictPhraseParser()
{
}

bool StrictPhraseParser::TrySplit(
                                  const std::wstring& phrase,
                                  list<unsigned int>& conds,
                                  list<std::wstring>& splitPhrase)
{
    StrictPhraseStateMachine sm;
    
    std::wstring::const_iterator it = phrase.begin();
    while(it != phrase.end()) {
        if(ShouldIgnore(*it)) {
            continue;
        }
        
        if( PhraseStateMachineNoError != sm.Input(*it)) {
            return false;
        }
        
        it++;
    }
    
    
    
    return true;
}

bool StrictPhraseParser::ShouldIgnore(wchar_t letter)
{
    bool shouldIgnore = true;
    switch(letter) {
        case L'\n':
        case L'\r':
        case L' ':
        case L'\t':
            break;
        default:
            break;
    }
    return shouldIgnore;
}
