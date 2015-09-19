//
//  VoiceControllerTest.m
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/11.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "MikuPhrase.h"
#include "MappedVoiceController.h"
#include "StrictVoiceController.h"
#include "AvoidTooShortVoiceController.h"
#include "VoiceTimingStopWatch.h"
#include "PronouncableLetterMap.h"
#include "StrictPhraseParser.h"
#include "Utilities.h"
#include <vector>

using namespace std;

@interface VoiceControllerTest : XCTestCase

@end

@implementation VoiceControllerTest
{
    string _path;
}

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    if(!PronouncableLetterMap::Instance().IsInitialized()) {
        NSBundle *bundle = [NSBundle bundleForClass:[self class]];
        NSString* path = [bundle pathForResource:@"pm-char-map" ofType:@"txt"];
        _path = string([path UTF8String]);
        PronouncableLetterMap::Instance().Initialize(_path.c_str());
    }
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testNormalPhrasing {
    
    wstring input = L"あいうえお";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    wstring letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"い") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"う") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"え") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"お") == 0);
}

- (void)testNormalNonCirculatePhraseEnding {
    wstring input = L"あ";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    mp.SetCirculation(false);
    wstring letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"") == 0);
}

- (void)testNormalCirculatePhraseEnding {
    wstring input = L"あい";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    mp.SetCirculation(true);
    wstring letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"い") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"あ") == 0);
}

- (void)testVowelPhrasing {
    wstring input = L"あぁきゃぇ";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    wstring letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"きゃ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare(L"あ") == 0);
}

- (void)testMappedVoiceControllerStartedEvent {
    string input = "ぅいあぁえ";
    MappedVoiceController vc;
    VoiceControllerNotification notif;
    
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetPhrase(input);
    vc.SetThreshold(10, 10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    vc.Input(20,50,notif);
    // callback expected
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    XCTAssertTrue(notif.level == 20);
    XCTAssertTrue(notif.note == 50);
    XCTAssertTrue(notif.pronounciation == "い");
}

- (void)testMappedVoiceControllerFinishEvent {
    string input = "ぅいあぁえ";
    MappedVoiceController vc;
    VoiceControllerNotification notif;
    
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetPhrase(input);
    vc.SetThreshold(10, 10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    vc.Input(20,50,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    
    vc.Input(9,10,notif);
    // callback expected
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(notif.level == 9);
    XCTAssertTrue(notif.note == kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
}

- (void)testMappedVoiceControllerChangeEvent {
    string input = "ぅいあぁえ";
    MappedVoiceController vc;
    VoiceControllerNotification notif;
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetThreshold(10,10);
    vc.SetPhrase(input);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    vc.Input(20,50,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    
    vc.Input(100,45,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceChanged);
    XCTAssertTrue(notif.level == 100);
    XCTAssertTrue(notif.note == 45);
    XCTAssertTrue(notif.pronounciation == "あ");// "あぁ" is not pronouncable
}

- (void)testMappedoiceControllerNoEventAfterNoteOffEvenLevelIncrease {
    string input = "ぅいあぁえ";
    MappedVoiceController vc;
    VoiceControllerNotification notif;
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetPhrase(input);
    vc.SetThreshold(10,10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    vc.Input(20,50,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    
    vc.Input(9,0,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(notif.level == 9);
    XCTAssertTrue(notif.note == kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
    
    vc.Input(20, kNoMidiNote,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
}


- (void)testStrictPhraseParserNoNote {
    wstring input = L"どれみふぁそ";
    
    StrictPhraseParser parser;
    list<unsigned int> conds;
    list<std::wstring> split;
    
    if(!parser.TrySplit(input, conds, split)) {
        XCTFail("parse failed");
    }
    XCTAssertTrue(conds.size() == 5);
    XCTAssertTrue(split.size() == 5);
}

- (void)testStrictPhraseParserShartFlat {
    vector<wstring> inputPron = {L"ど",L"ど",L"れ",L"れ",L"み",L"ふぁ",L"ふぁ",L"そ",L"そ",L"ら",L"ら",L"し",L"ど"};
    vector<wstring> inputNote = {L"c",L"c#",L"c##",L"d#",L"e",L"f",L"f#",L"g",L"a♭",L"b♭♭",L"b♭",L"c♭",L"c"};

    wstring input;
    for(int i = 0; i < inputPron.size(); i++ ) {
        input += inputNote[i] + inputPron[i];
    }
    
    StrictPhraseParser parser;
    list<unsigned int> conds;
    list<std::wstring> split;
    
    if(!parser.TrySplit(input, conds, split)) {
        XCTFail("parse failed");
    }
    
    list<unsigned int>::const_iterator conds_it = conds.begin();
    list<std::wstring>::const_iterator split_it = split.begin();
    
    unsigned int counter = 0;
    while(conds_it != conds.end() && conds_it != conds.end()) {
        XCTAssertTrue(*conds_it == counter%12, @"*conds_it=%o expected=%o", *conds_it, counter%12);
        XCTAssertTrue(split_it->compare(inputPron[counter]) == 0, @"*split_it=%@, exptected=%@",
                      [NSString stringWithUTF8String:ws2s(*split_it).c_str()],
                      [NSString stringWithUTF8String:ws2s(inputPron[counter]).c_str()]);
        
        conds_it++;
        split_it++;
        counter++;
    }
}

- (void)testStrictPhraseParserSkipSpace {
    wstring input = L" \nふぁ F ふぁ Gそ\tひゃ\r\n";
    StrictPhraseParser parser;
    list<unsigned int> conds;
    list<unsigned int>::const_iterator conds_it;
    list<std::wstring> split;
    list<std::wstring>::const_iterator split_it;
    
    if(!parser.TrySplit(input, conds, split)) {
        XCTFail("parse failed");
    }
    
    conds_it = conds.begin();
    split_it = split.begin();
    
    XCTAssertTrue(*conds_it == kNoMidiNote);
    XCTAssertTrue(split_it->compare(L"ふぁ") == 0);
    
    conds_it++;
    split_it++;
    XCTAssertTrue(*conds_it == 5);
    XCTAssertTrue(split_it->compare(L"ふぁ") == 0);
    
    conds_it++;
    split_it++;
    XCTAssertTrue(*conds_it == 7);
    XCTAssertTrue(split_it->compare(L"そ") == 0);
}

- (void)testVoiceTimingStopWatch {
    VoiceTimingStopWatch sw;
    sw.Reset(100);
    XCTAssertFalse(sw.IsEllapsed());
    usleep(2000 * 1000);
    XCTAssertTrue(sw.IsEllapsed());
}

- (void)testStrictVoiceControllerStartedEvent {
    string input = "ぅcいeあぁgえ";
    StrictVoiceController vc;
    VoiceControllerNotification notif;
    
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetPhrase(input);
    vc.SetThreshold(10, 10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    vc.Input(20,48,notif);
    // callback expected
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    XCTAssertTrue(notif.level == 20);
    XCTAssertTrue(notif.note == 48);
    XCTAssertTrue(notif.pronounciation == "い");
}

- (void)testAvoidTooShortVoiceControllerFinishEvent {
    string input = "ぅいあぁえ";
    AvoidTooShortVoiceController vc;
    VoiceControllerNotification notif;
    bool responded = false;
    
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetMinimumVoiceLength(200);//200ms
    vc.SetPhrase(input);
    vc.SetThreshold(10, 10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    responded = vc.Input(20,50,notif);
    XCTAssertTrue(responded);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    XCTAssertTrue(notif.level == 20);
    XCTAssertTrue(notif.note == 50);
    XCTAssertTrue(notif.pronounciation == "い");
    
    responded = vc.Input(0,kNoMidiNote, notif);
    XCTAssertFalse(responded);//should not respoind to short on->off
    
    usleep(210*1000);// sleep 210ms
    responded = vc.Input(0,kNoMidiNote, notif);
    XCTAssertTrue(responded);// should respond
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(notif.level == 0);
    XCTAssertTrue(notif.note == kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
    
    responded = vc.Input(9,10,notif);
    XCTAssertFalse(responded);//should not respoind since not enough level
}

- (void)testAvoidTooShortVoiceControllerChangeEvent {
    string input = "ぅいあぁえ";
    AvoidTooShortVoiceController vc;
    VoiceControllerNotification notif;
    bool responded = false;
    
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    vc.SetMinimumVoiceLength(200);//200ms
    vc.SetPhrase(input);
    vc.SetThreshold(10, 10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    responded = vc.Input(20,50,notif);
    XCTAssertTrue(responded);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    XCTAssertTrue(notif.level == 20);
    XCTAssertTrue(notif.note == 50);
    XCTAssertTrue(notif.pronounciation == "い");
    
    responded = vc.Input(20,52, notif);
    XCTAssertFalse(responded);//should not respoind since to shoort to change
    
    usleep(210*1000);
    responded = vc.Input(20,54, notif);
    XCTAssertTrue(responded);// should respond
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceChanged);
    XCTAssertTrue(notif.level == 20);
    XCTAssertTrue(notif.note == 54);
    XCTAssertTrue(notif.pronounciation == "あ");
    
    responded = vc.Input(9,10,notif);
    XCTAssertFalse(responded);//should not respoind since not enough level
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
