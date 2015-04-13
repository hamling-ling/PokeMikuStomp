//
//  StaticVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "StaticVoiceController.h"
#include "MikuPhrase.h"
#include <fstream>
#include "Utilities.h"

using namespace std;

StaticVoiceController::StaticVoiceController()
{
    
}

StaticVoiceController::~StaticVoiceController()
{
    
}

bool StaticVoiceController::MakePronounceStringMap(const char* path, std::map<std::wstring, int>& map) {
    ifstream file(path);
    if(!file.is_open()) {
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        wstring wline = s2ws(line);
        pair<wstring, int> item(wline, count++);
        map.insert(item);
    }
    file.close();
    
    return true;
}

bool StaticVoiceController::SetPhrase(std::string& phrase, const char* mapPath)
{
    if(!MakeMap(mapPath)) {
        return false;
    }
    
    shared_ptr<MikuPhrase> mikuPhrase = make_shared<MikuPhrase>(phrase, _proMap);
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
    
    return true;
}

bool StaticVoiceController::HandleInputLevelToOff(int level, VoiceControllerNotification& notif)
{
    bool isOn = (kNoMidiNote != _currentNote);
    // update level
    _currentInputLevel = level;
    
    if( isOn && IsBelowOnToOff()) {
        _currentPronounciation = "";
        _currentNote = kNoMidiNote;
        
        // note off
        notif = MakeFinishedNotification();
        return true;
    }
    
    return false;
}

bool StaticVoiceController::Input(int level, unsigned int note, VoiceControllerNotification& notif)
{
    if(HandleInputLevelToOff(level, notif)) {
        return true;
    }
    
    if(note == kNoMidiNote) {
        return false;
    }
    
    if(!IsAboveOffToOn()) {
        return false;
    }
    
    if(_currentNote == kNoMidiNote) {
        _currentNote = note;
        notif = MakeStartedNotification();
        return true;
    }
    
    if(_currentNote != note) {
        _currentNote = note;
        notif = MakeChangedNotification();
        return true;
    }
    
    return false;
}

bool StaticVoiceController::MakeMap(const char* mapPath)
{
    return MakePronounceStringMap(mapPath, _proMap);
}
