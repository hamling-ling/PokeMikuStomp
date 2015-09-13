//
//  StrictPhraseStateContext.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/13.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__StrictPhraseStateContext__
#define __PokeMikuStompLib__StrictPhraseStateContext__

#include <list>
#include <string>
#include "VoiceControllerCommon.h"

struct StrictPhraseStateContext {
public:
    unsigned int note;  // lowest midi note# of the note
    int mod;            // -1=flat,0=natural,+1=sharp
    wchar_t letter;     // letter
    wchar_t small;      // small vowel
    std::list<unsigned int> notes;
    std::list<std::wstring> pronounces;
    
    StrictPhraseStateContext();
    bool IsValid();
    bool IsAppendableSmallVowel(wchar_t letter);
    unsigned int GetMidiNote();
    bool Push();
    static unsigned int NoteFromLetter(wchar_t letter);
    static bool IsNote(wchar_t letter);
};

#endif /* defined(__PokeMikuStompLib__StrictPhraseStateContext__) */
