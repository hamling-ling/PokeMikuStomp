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
#include "PronouncableLetterMap.h"

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
    
    string input = "あいうえお";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    string letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("い") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("う") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("え") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("お") == 0);
}

- (void)testNormalNonCirculatePhraseEnding {
    string input = "あ";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    mp.SetCirculation(false);
    string letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("") == 0);
}

- (void)testNormalCirculatePhraseEnding {
    string input = "あい";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    mp.SetCirculation(true);
    string letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("い") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
}

- (void)testVowelPhrasing {
    string input = "あぁきゃぇ";
    XCTAssertFalse(!PronouncableLetterMap::Instance().IsInitialized());
    MikuPhrase mp(input);
    string letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("きゃ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
}

- (void)testStaticVoiceControllerStartedEvent {
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

- (void)testStaticVoiceControllerFinishEvent {
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
    XCTAssertTrue(notif.note == VoiceController::kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
}

- (void)testStaticVoiceControllerChangeEvent {
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

- (void)testStaticVoiceControllerNoEventAfterNoteOffEvenLevelIncrease {
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
    XCTAssertTrue(notif.note == VoiceController::kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
    
    vc.Input(20, VoiceController::kNoMidiNote,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
