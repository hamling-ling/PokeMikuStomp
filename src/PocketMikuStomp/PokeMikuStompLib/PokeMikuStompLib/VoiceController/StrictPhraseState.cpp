//
//  StrictPhraseState.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/13.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StrictPhraseState.h"
#include <cassert>

using namespace std;

//
// StrictPhraseState
//

StrictPhraseState::StrictPhraseState(){}

StrictPhraseState::~StrictPhraseState(){}

bool StrictPhraseState::IsError() {
    return false;
}

StrictPhraseState* StrictPhraseState::OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    
    if(pro.IsValid()) {
        pro.Push();
    }
    
    pro.note  = StrictPhraseStateContext::NoteFromLetter(letter);
    return _allStates[StrictPhraseStateIdNote];
}

StrictPhraseState* StrictPhraseState::OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    pro.mod += 1;
    return _allStates[StrictPhraseStateIdMod];
}

StrictPhraseState* StrictPhraseState::OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    pro.mod -= 1;
    return _allStates[StrictPhraseStateIdMod];
}

StrictPhraseState* StrictPhraseState::OnLetterEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    pro.letter = letter;
    return _allStates[StrictPhraseStateIdLetter];
}

StrictPhraseState* StrictPhraseState::OnSvowelEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    pro.small = letter;
    return _allStates[StrictPhraseStateIdSvowel];
}

StrictPhraseState* StrictPhraseState::OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    pro.Push();
    return _allStates[StrictPhraseStateIdNote];
}

void StrictPhraseState::SetAllStates(StrictPhraseState* states[StrictPhraseStateIdMax]) {
    memcpy(_allStates, states, sizeof(StrictPhraseState*) * StrictPhraseStateIdMax);
}

//
// NoteState
//

NoteState::NoteState(){}

NoteState::~NoteState(){}

StrictPhraseState* NoteState::OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    pro.Push();
    return _allStates[StrictPhraseStateIdError];
}

//
// ModState
//

ModState::ModState(){}

ModState::~ModState(){}

StrictPhraseState* ModState::OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

StrictPhraseState* ModState::OnSvowelEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

StrictPhraseState* ModState::OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

//
// LetterState
//

LetterState::LetterState(){}

LetterState::~LetterState(){}

StrictPhraseState* LetterState::OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

StrictPhraseState* LetterState::OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

//
// SvState
//

SvState::SvState(){}

SvState::~SvState(){}

StrictPhraseState* SvState::OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

StrictPhraseState* SvState::OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

StrictPhraseState* SvState::OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

StrictPhraseState* SvState::OnLetterEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return _allStates[StrictPhraseStateIdError];
}

//
// ErrState
//

ErrState::ErrState(){}

ErrState::~ErrState(){}

bool ErrState::IsError() {
    return true;
}

StrictPhraseState* ErrState::OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return this;
}

StrictPhraseState* ErrState::OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return this;
}

StrictPhraseState* ErrState::OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return this;
}

StrictPhraseState* ErrState::OnLetterEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return this;
}

StrictPhraseState* ErrState::OnSvowelEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return this;
}

StrictPhraseState* ErrState::OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter) {
    return this;
}

