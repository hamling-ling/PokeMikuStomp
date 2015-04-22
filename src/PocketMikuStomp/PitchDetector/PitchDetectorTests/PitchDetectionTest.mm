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

static NSString* const kKeyFilename = @"fileName";
static NSString* const kKeyRate = @"rate";
static NSString* const kKeySampleNum = @"sampleNum";
static NSString* const kKeyNote = @"note";

@interface PitchDetectionTest : XCTestCase
@end

@implementation PitchDetectionTest

- (void)setUp {
	[super setUp];
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testTable {
    NSArray* testTable = @[@{kKeyFilename:@"piano_mid_A",  kKeyRate:@(8000),  kKeySampleNum:@(1024), kKeyNote:@(57)},
                         @{kKeyFilename:@"guitar_low_E",   kKeyRate:@(22050), kKeySampleNum:@(1024), kKeyNote:@(40)},
                         @{kKeyFilename:@"trumpet_low_Bb", kKeyRate:@(22050), kKeySampleNum:@(1024), kKeyNote:@(58)},
                         @{kKeyFilename:@"trumpet_mid_F",  kKeyRate:@(22050), kKeySampleNum:@(1024), kKeyNote:@(65)},
                         @{kKeyFilename:@"trumpet_mid_Bb", kKeyRate:@(22050), kKeySampleNum:@(1024), kKeyNote:@(70)},
                         @{kKeyFilename:@"trumpet_upper_D",kKeyRate:@(22050), kKeySampleNum:@(1024), kKeyNote:@(74)},
                         @{kKeyFilename:@"trumpet_upper_F",kKeyRate:@(22050), kKeySampleNum:@(1024), kKeyNote:@(77)}
                            ];
    
    for(NSDictionary* dict in testTable) {
        [self testWithTableDict:dict];
    }
}

- (bool)testWithTableDict:(NSDictionary*)dict {
    NSString* fileName = dict[kKeyFilename];
    int rate = [dict[kKeyRate] intValue];
    int sampleNum = [dict[kKeySampleNum] intValue];
    int note = [dict[kKeyNote] intValue];
    
    PitchInfo pitch;
    if(![self testWithFileName:fileName rate:rate sampleNum:sampleNum pitch:pitch]) {
        NSLog(@"test for %@ failed!", fileName);
        return false;
    }
    
    if(pitch.error) {
        NSLog(@"test for %@ failed!", fileName);
        XCTAssertFalse(pitch.error);
        return false;
    }
    
    if(pitch.midi != note) {
        NSLog(@"test for %@ failed!", fileName);
        XCTAssertTrue(pitch.midi == note);
    }
    
    [self printPitch:pitch];
    return true;
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

- (bool)testWithFileName:(NSString*)fileName rate:(const int)rate sampleNum:(int)sampleNum pitch:(PitchInfo&)pitch {
    float* data = new float[sampleNum];
    memset(data, 0, sizeof(float) * sampleNum);
    
    if(![self loadData:fileName into:data len:sampleNum]) {
        return false;
    }
    
    PitchDetector detector(rate, sampleNum);
    XCTAssertTrue(detector.Initialize());
    XCTAssertTrue(detector.Detect(data));
    
    detector.GetPiatch(pitch);
    return true;
}

- (void)printPitch:(PitchInfo&)pitch {
    cout << "result---" << endl;
    cout << "freq    :" << pitch.freq << endl;
    cout << "note    :" << pitch.midi << endl;
    cout << "noteStr :" << pitch.noteStr << endl;
    cout << "octave  :" << int(pitch.octave) << endl;
}

@end
