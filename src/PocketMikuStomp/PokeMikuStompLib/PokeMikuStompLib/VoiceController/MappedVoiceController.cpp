//
//  MappedVoiceController.cpp
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "MappedVoiceController.h"
#include "MikuPhrase.h"
#include <fstream>
#include "Utilities.h"

using namespace std;

MappedVoiceController::MappedVoiceController()
{
    
}

MappedVoiceController::~MappedVoiceController()
{
    
}

bool MappedVoiceController::MakePronounceStringMap(const char* path, std::map<std::wstring, int>& map) {
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

bool MappedVoiceController::SetMap(const char* mapPath)
{
    return MakeMap(mapPath);
}

bool MappedVoiceController::SetPhrase(std::string& phrase)
{
    std::lock_guard<std::recursive_mutex> lock(_phraseMutex);
    
    shared_ptr<MikuPhrase> mikuPhrase = make_shared<MikuPhrase>(phrase, _proMap);
    _phrase = std::dynamic_pointer_cast<Phrase>(mikuPhrase);
    
    return true;
}

bool MappedVoiceController::MakeMap(const char* mapPath)
{
    return MakePronounceStringMap(mapPath, _proMap);
}
