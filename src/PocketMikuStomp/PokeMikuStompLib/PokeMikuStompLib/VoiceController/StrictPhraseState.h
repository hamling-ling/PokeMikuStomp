//
//  StrictPhraseState.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/09/13.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PokeMikuStompLib__StrictPhraseState__
#define __PokeMikuStompLib__StrictPhraseState__

#include "StrictPhraseStateContext.h"

enum StrictPhraseStateId {
    StrictPhraseStateIdNote,
    StrictPhraseStateIdMod,
    StrictPhraseStateIdLetter,
    StrictPhraseStateIdSvowel,
    StrictPhraseStateIdError,
    StrictPhraseStateIdMax,
};

struct StrictPhraseState
{
public:
    StrictPhraseState();
    virtual ~StrictPhraseState();
    virtual bool IsError();
    virtual StrictPhraseState* OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnLetterEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnSvowelEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter);
    void SetAllStates(StrictPhraseState* states[]);
protected:
    StrictPhraseState* _allStates[StrictPhraseStateIdMax];
};

struct NoteState : public StrictPhraseState
{
public:
    NoteState();
    virtual ~NoteState();
    virtual StrictPhraseState* OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter);
};

struct ModState : public StrictPhraseState
{
public:
    ModState();
    virtual ~ModState();
    virtual StrictPhraseState* OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnSvowelEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter);
};

struct LetterState : public StrictPhraseState
{
public:
    LetterState();
    virtual ~LetterState();
    virtual StrictPhraseState* OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter);
};

struct SvState : public StrictPhraseState
{
public:
    SvState();
    virtual ~SvState();
    virtual StrictPhraseState* OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnLetterEvt(StrictPhraseStateContext& pro, wchar_t letter);
};

struct ErrState : public StrictPhraseState
{
public:
    ErrState();
    virtual ~ErrState();
    virtual bool IsError();
    virtual StrictPhraseState* OnNoteEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnSharpEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnFlatEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnLetterEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnSvowelEvt(StrictPhraseStateContext& pro, wchar_t letter);
    virtual StrictPhraseState* OnEofEvt(StrictPhraseStateContext& pro, wchar_t letter);
};

#endif /* defined(__PokeMikuStompLib__StrictPhraseState__) */
