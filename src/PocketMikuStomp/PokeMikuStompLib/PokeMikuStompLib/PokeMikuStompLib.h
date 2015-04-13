//
//  PokeMikuStompLib.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM (NSUInteger, PokeMikuStompLibError) {
    kPokeMikuStompLibNoError,
    kPokeMikuStompLibErrorPokeMikuNotFound,
    kPokeMikuStompLibErrorNotInitialized,
    kPokeMikuStompLibErrorNoCaptureDevice,
    kPokeMikuStompLibErrorCaptureDeviceExist,
    kPokeMikuStompLibErrorCaptureNotInitialized,
    kPokeMikuStompLibErrorCaptureRunning,
    kPokeMikuStompLibErrorInternalError,
};

@interface PokeMikuStompLib : NSObject

@property (atomic, readwrite, assign) int levelThreshold;
@property (atomic, readonly, assign) int inputLevel;
@property (atomic, readonly, assign) int midiNote;
@property (atomic, readonly, strong) NSString* noteString;
@property (atomic, readonly, strong) NSString* pronoString;
@property (atomic, readwrite, assign) int OffToOnThreshold;
@property (atomic, readwrite, assign) int OnToOffThreshold;

- (NSArray*)devices;
- (PokeMikuStompLibError)selectDeviceWithId:(int)deviceId;
- (PokeMikuStompLibError)setup;
- (void)teardown;
- (PokeMikuStompLibError)start;
- (void)stop;
- (void)test;
@end
