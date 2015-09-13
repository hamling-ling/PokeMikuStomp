//
//  StrictPhraseStateMachine.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__StrictPhraseStateMachine__
#define __PokeMikuStompLib__StrictPhraseStateMachine__

#include <memory>
#include "StrictPhraseState.h"

enum PhraseStateMachineError {
    PhraseStateMachineNoError,
    PhraseStateMachineInvalidInput,
};

/**
 *  state init  :initial state.
 *  state note  :waiting for note or already has note and waiting for shar/flat or miku letter.
 *  state mod   :already has sharp or flat and waiting for miku letter.
 *  state letter:already has letter and waiting for small vowel, next note, or next letter
 *  state svowel:already has small vowel and waiting for next note, next letter.
 *  state EOF   :end of input string.
 *                  state
 *                  note    mod     letter  svowel  err
 *  note evt        note    err     note    err     err
 *  sharp evt       mod     mod     err     err     err
 *  flat evt        mod     mod     err     err     err
 *  letter evt      letter  letter  letter  err     err
 *  small vowel evt err     err     svowel  svowel  err
 *  EOF             err     err     note    note    err
 *
 */
class StrictPhraseStateMachine
{
public:
    StrictPhraseStateMachine();
    ~StrictPhraseStateMachine();
    PhraseStateMachineError Input(wchar_t letter);
private:
    StrictPhraseStateContext _ctx;
    StrictPhraseState* states[StrictPhraseStateIdMax];
    StrictPhraseState*  _state;
    bool IsNote(wchar_t letter);
};

#endif /* defined(__PokeMikuStompLib__StrictPhraseStateMachine__) */
