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

static const int dataNum = 1024;
static float data[dataNum] = {0.0f};

using namespace std;

@interface PitchDetectionTest : XCTestCase
{
}
@end

@implementation PitchDetectionTest

+ (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
	NSBundle *bundle = [NSBundle bundleForClass:[self class]];
	NSString* path = [bundle pathForResource:@"file_orig" ofType:@"csv"];
	XCTAssertNotNil(path, @"no bundle path");
	NSLog(@"%@", path);
	
	
	ifstream file([path UTF8String]);
	XCTAssertTrue(file.is_open());
	
	string line;
	int index = 0;
	while (getline(file, line) && index < dataNum) {
		istringstream iss(line);
		float x;
		if (!(iss >> x)) {
			XCTFail(@"load file error");
		}
		data[index++] = x;
	}
	file.close();
}

- (void)setUp {
	[super setUp];
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    // This is an example of a functional test case.
	
	PitchDetector detector(8000, dataNum);
	XCTAssertTrue(detector.Initialize());
	
	XCTAssertTrue(detector.Detect(data));
	
	PitchInfo pitch = { 0 };
	detector.GetPiatch(pitch);
	
	cout << "result---" << endl;
	cout << "freq    :" << pitch.freq << endl;
	cout << "note    :" << pitch.note << endl;
	cout << "noteStr :" << pitch.noteStr << endl;
	cout << "octabe  :" << int(pitch.octave) << endl;
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
