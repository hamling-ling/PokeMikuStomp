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
#include "StaticVoiceController.h"

using namespace std;

@interface VoiceControllerTest : XCTestCase

@end

@implementation VoiceControllerTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testNormalPhrasing {
    
    string input = "あいうえお";
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
    string input = "あぁいぃぇ";
    MikuPhrase mp(input);
    string letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あぁ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("いぃ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("ぇ") == 0);
}

- (void)testStaticVoiceControllerStartedEvent {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    VoiceControllerNotification notif;
    
    svc.SetPhrase(input);
    svc.SetThreshold(10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.Input(20,50,notif);
    // callback expected
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    XCTAssertTrue(notif.level == 20);
    XCTAssertTrue(notif.note == 50);
    XCTAssertTrue(notif.pronounciation == "ぅ");
}

- (void)testStaticVoiceControllerFinishEvent {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    VoiceControllerNotification notif;
    
    svc.SetPhrase(input);
    svc.SetThreshold(10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.Input(20,50,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    
    svc.Input(9,10,notif);
    // callback expected
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(notif.level == 9);
    XCTAssertTrue(notif.note == StaticVoiceController::kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
}

- (void)testStaticVoiceControllerChangeEvent {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    VoiceControllerNotification notif;
    svc.SetPhrase(input);
    svc.SetThreshold(10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.Input(20,50,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    
    svc.Input(100,45,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceChanged);
    XCTAssertTrue(notif.level == 100);
    XCTAssertTrue(notif.note == 45);
    XCTAssertTrue(notif.pronounciation == "い");
}

- (void)testStaticVoiceControllerNoEventAfterNoteOffEvenLevelIncrease {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    VoiceControllerNotification notif;
    svc.SetPhrase(input);
    svc.SetThreshold(10);
    notif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.Input(20,50,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceStarted);
    
    svc.Input(9,0,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(notif.level == 9);
    XCTAssertTrue(notif.note == StaticVoiceController::kNoMidiNote);
    XCTAssertTrue(notif.pronounciation == "");
    
    svc.Input(20, VoiceController::kNoMidiNote,notif);
    XCTAssertTrue(notif.type == VoiceControllerNotificationTypePronounceFinished);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
