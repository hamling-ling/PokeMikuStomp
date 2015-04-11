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

VoiceControllerNotification gNotif;

static void voiceControllerCallback(const VoiceControllerNotification& notif) {
    gNotif = notif;
}

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
    svc.SetPhrase(input);
    svc.SetCallback(voiceControllerCallback, NULL);
    svc.SetThreshold(10);
    gNotif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.InputLevel(20);
    // no callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceFinished);
    svc.InputNote(50);
    // callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceStarted);
    XCTAssertTrue(gNotif.level == 20);
    XCTAssertTrue(gNotif.note == 50);
    XCTAssertTrue(gNotif.pronounciation == "ぅ");
}

- (void)testStaticVoiceControllerFinishEvent {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    svc.SetPhrase(input);
    svc.SetCallback(voiceControllerCallback, NULL);
    svc.SetThreshold(10);
    gNotif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.InputLevel(20);
    svc.InputNote(50);
    svc.InputLevel(19);
    // no callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceStarted);
    
    svc.InputLevel(9);
    // callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(gNotif.level == 9);
    XCTAssertTrue(gNotif.note == StaticVoiceController::kNoMidiNote);
    XCTAssertTrue(gNotif.pronounciation == "");
}

- (void)testStaticVoiceControllerChangeEvent {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    svc.SetPhrase(input);
    svc.SetCallback(voiceControllerCallback, NULL);
    svc.SetThreshold(10);
    gNotif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.InputLevel(20);
    svc.InputNote(50);
    svc.InputLevel(19);
    // no callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceStarted);
    
    svc.InputLevel(100);
    svc.InputNote(45);
    // callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceChanged);
    XCTAssertTrue(gNotif.level == 100);
    XCTAssertTrue(gNotif.note == 45);
    XCTAssertTrue(gNotif.pronounciation == "い");
}

- (void)testStaticVoiceControllerNoEventAfterNoteOffEvenLevelIncrease {
    string input = "ぅいあぁえ";
    StaticVoiceController svc;
    svc.SetPhrase(input);
    svc.SetCallback(voiceControllerCallback, NULL);
    svc.SetThreshold(10);
    gNotif.type = VoiceControllerNotificationTypePronounceFinished;
    
    svc.InputLevel(20);
    svc.InputNote(50);
    svc.InputLevel(19);
    // no callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceStarted);
    
    svc.InputLevel(9);
    // callback expected
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceFinished);
    XCTAssertTrue(gNotif.level == 9);
    XCTAssertTrue(gNotif.note == StaticVoiceController::kNoMidiNote);
    XCTAssertTrue(gNotif.pronounciation == "");
    
    svc.InputLevel(20);
    XCTAssertTrue(gNotif.type == VoiceControllerNotificationTypePronounceFinished);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
