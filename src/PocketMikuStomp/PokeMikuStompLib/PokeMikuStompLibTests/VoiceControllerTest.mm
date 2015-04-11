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
    string input = "あぅい";
    MikuPhrase mp(input);
    string letters;
    
    letters = mp.Next();
    XCTAssertTrue(letters.compare("あ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("ぅ") == 0);
    letters = mp.Next();
    XCTAssertTrue(letters.compare("い") == 0);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
