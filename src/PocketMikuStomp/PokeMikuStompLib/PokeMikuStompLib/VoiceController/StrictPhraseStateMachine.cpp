//
//  StrictPhraseStateMachine.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/12.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictPhraseStateMachine.h"
#include "PronouncableLetterMap.h"

using namespace std;


StrictPhraseStateMachine::StrictPhraseStateMachine() {
    memset(&states[0], NULL, sizeof(states));
    
    states[StrictPhraseStateIdNote] = new NoteState();
    states[StrictPhraseStateIdMod] = new ModState();
    states[StrictPhraseStateIdLetter] = new LetterState();
    states[StrictPhraseStateIdSvowel] = new SvState();
    states[StrictPhraseStateIdError] = new ErrState();
    
    for(int i = 0; i < StrictPhraseStateIdMax; i++) {
        states[i]->SetAllStates(&(states[0]));
    }
    
    _state = states[0];
}

StrictPhraseStateMachine::~StrictPhraseStateMachine() {
    for(int i = 0; i < StrictPhraseStateIdMax; i++) {
        StrictPhraseState* state = states[i];
        states[i] = NULL;
        delete state;
    }
    _state = NULL;
}

PhraseStateMachineError StrictPhraseStateMachine::Input(wchar_t letter)
{
    PronouncableLetterMap &map = PronouncableLetterMap::Instance();
    if(StrictPhraseStateContext::IsNote(letter)) {
        _state = _state->OnNoteEvt(_ctx, letter);
    } else if(L'#' == letter) {
        _state = _state->OnSharpEvt(_ctx, letter);
    } else if(L'♭' == letter) {
        _state = _state->OnFlatEvt(_ctx, letter);
    } else if(map.IsPronounsableLetter(letter)) {
        _state = _state->OnLetterEvt(_ctx, letter);
    } else if(map.IsSmallVowel(letter)) {
        _state = _state->OnSvowelEvt(_ctx, letter);
    } else if(L'\0' == letter) {
        _state = _state->OnEofEvt(_ctx, letter);
    }

    if(_state->IsError()) {
        return PhraseStateMachineInvalidInput;
    }
    return PhraseStateMachineNoError;
}

void StrictPhraseStateMachine::GetResult(list<unsigned int>& notes, list<std::wstring>& pros)
{
    notes.clear();
    notes.assign(_ctx.notes.begin(), _ctx.notes.end());
    
    pros.clear();
    pros.assign(_ctx.pronounces.begin(), _ctx.pronounces.end());
}
