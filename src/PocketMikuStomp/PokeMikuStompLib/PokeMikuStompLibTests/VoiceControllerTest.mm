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
#include "VoiceController.h"
#include "MappedVoiceController.h"
#include "StrictVoiceController.h"
#include "PronouncableLetterMap.h"
#include "StrictPhraseParser.h"

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

- (void)testStrictPhraseParser {
    wstring input = L"cどdれeみ";
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
    
    XCTAssertTrue(*conds_it == 0);
    XCTAssertTrue(split_it->compare(L"ど") == 0);
    
    conds_it++;
    split_it++;
    XCTAssertTrue(*conds_it == 2);
    XCTAssertTrue(split_it->compare(L"れ") == 0);
    
    conds_it++;
    split_it++;
    XCTAssertTrue(*conds_it == 4);
    XCTAssertTrue(split_it->compare(L"み") == 0);
}

- (void)testStrictVoiceControllerStartedEvent {
    // t.b.d
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
