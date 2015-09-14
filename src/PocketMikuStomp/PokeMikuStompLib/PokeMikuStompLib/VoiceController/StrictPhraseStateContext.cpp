//
//  StrictPhraseStateContext.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/13.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictPhraseStateContext.h"
#include "PronouncableLetterMap.h"
#include <cassert>
#include <string>

using namespace std;

StrictPhraseStateContext::StrictPhraseStateContext() : note(kNoMidiNote), mod(0), letter(0), small(0) {}

bool StrictPhraseStateContext::IsValid() {
    if(!PronouncableLetterMap::Instance().IsPronounsableLetter(letter)) {
        return false;
    }
    if(L'\0' != small) {
        if(!PronouncableLetterMap::Instance().IsSmallVowel(small)) {
            return false;
        }
    }
    
    wchar_t proArr[3] = {letter, small, L'\0'};
    std::wstring pro(proArr);
    if(!PronouncableLetterMap::Instance().IsPronounsableLetter(pro)) {
        return false;
    }
    
    if(mod < -2 || 2 < mod) {
        return false;
    }
    return true;
}

bool StrictPhraseStateContext::IsAppendableSmallVowel(wchar_t smallVowel)
{
    if(L'\0' == letter) {
        return false;
    }
    
    if(L'\0' == smallVowel) {
        return false;
    }
    
    wchar_t proArr[3] = {letter, smallVowel, L'\0'};
    std::wstring pro(proArr);
    if(!PronouncableLetterMap::Instance().IsPronounsableLetter(pro)) {
        return false;
    }
    return true;
}

unsigned int StrictPhraseStateContext::GetMidiNote() {
    if(note == kNoMidiNote) {
        return note;
    }
    
    return ((note + 12) + mod) % 12;
}

bool StrictPhraseStateContext::Push() {
    if(!IsValid()) {
        return false;
    }
    notes.push_back(GetMidiNote());
    wchar_t strarr[3] = {letter, small, L'\0'};
    pronounces.push_back(strarr);
    
    note = kNoMidiNote;
    mod = 0;
    letter = L'\0';
    small = L'\0';
    
    return true;
}

unsigned int StrictPhraseStateContext::NoteFromLetter(wchar_t letter) {
    unsigned int note = 0;
    switch(letter) {
        case L'c':
        case L'C':
            note = 0;
            break;
        case L'd':
        case L'D':
            note = 2;
            break;
        case L'e':
        case L'E':
            note = 4;
            break;
        case L'f':
        case L'F':
            note = 5;
            break;
        case L'g':
        case L'G':
            note = 7;
            break;
        case L'a':
        case L'A':
            note = 9;
            break;
        case L'b':
        case L'B':
            note = 11;
            break;
        default:
            assert(false);
            break;
    }
    return note;
}

bool StrictPhraseStateContext::IsNote(wchar_t letter) {
    bool result = false;
    switch(letter) {
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'a':
        case 'A':
        case 'b':
        case 'B':
            result = true;
            break;
        default:
            break;
    }
    return result;
}
