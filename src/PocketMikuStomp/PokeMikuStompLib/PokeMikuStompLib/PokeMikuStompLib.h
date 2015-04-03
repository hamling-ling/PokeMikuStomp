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
- (NSArray*)devices;
- (PokeMikuStompLibError)selectDeviceWithId:(int)deviceId;
- (PokeMikuStompLibError)setup;
- (void)teardown;
- (PokeMikuStompLibError)start;
- (void)stop;
- (void)test;
@end
