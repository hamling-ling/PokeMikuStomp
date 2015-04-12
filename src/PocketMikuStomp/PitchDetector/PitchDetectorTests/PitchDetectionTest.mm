//
//  PitchDetectionTest.m
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/08.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "PitchDetector.h"

using namespace std;

@interface PitchDetectionTest : XCTestCase
{
}
@end

@implementation PitchDetectionTest

- (void)setUp {
	[super setUp];
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testWithPianoMidA {
    NSString* fileName = @"piano_mid_A";
	
    PitchInfo pitch;
    [self testWithFileName:fileName rate:8000 sampleNum:1024 pitch:pitch];

	if(pitch.error) {
		XCTAssertFalse(pitch.error);
		return;
	}
	
    XCTAssertTrue(pitch.midi == 57);
    
    [self printPitch:pitch];
}

- (void)testWithGuiterLowE {
    NSString* fileName = @"guitar_low_E";
    
    PitchInfo pitch;
    [self testWithFileName:fileName rate:22050 sampleNum:1024 pitch:pitch];
    
    if(pitch.error) {
        XCTAssertFalse(pitch.error);
        return;
    }
    
    XCTAssertTrue(pitch.midi == 40);
    
    [self printPitch:pitch];
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

- (BOOL)loadData:(NSString*)fileName into:(float*)data len:(int)len {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString* path = [bundle pathForResource:fileName ofType:@"csv"];
    XCTAssertNotNil(path, @"no bundle path");
    NSLog(@"%@", path);
    
    
    ifstream file([path UTF8String]);
    XCTAssertTrue(file.is_open());
    
    string line;
    int index = 0;
    while (getline(file, line) && index < len) {
        istringstream iss(line);
        float x;
        if (!(iss >> x)) {
            XCTFail(@"load file error");
        }
        data[index++] = x;
    }
    file.close();
}

- (void)testWithFileName:(NSString*)fileName rate:(const int)rate sampleNum:(int)sampleNum pitch:(PitchInfo&)pitch {
    float* data = new float[sampleNum];
    memset(data, 0, sizeof(float) * sampleNum);
    
    [self loadData:fileName into:data len:sampleNum];
    
    PitchDetector detector(rate, sampleNum);
    XCTAssertTrue(detector.Initialize());
    XCTAssertTrue(detector.Detect(data));
    
    detector.GetPiatch(pitch);
}

- (void)printPitch:(PitchInfo&)pitch {
    cout << "result---" << endl;
    cout << "freq    :" << pitch.freq << endl;
    cout << "note    :" << pitch.midi << endl;
    cout << "noteStr :" << pitch.noteStr << endl;
    cout << "octave  :" << int(pitch.octave) << endl;
}

@end
