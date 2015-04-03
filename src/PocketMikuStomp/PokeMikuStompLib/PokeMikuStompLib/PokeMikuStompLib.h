//
//  PokeMikuStompLib.h
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PokeMikuStompLib : NSObject
- (NSArray*)devices;
- (int)selectDeviceWithId:(int)deviceId;
- (int)setup;
- (int)teardown;
- (int)start;
- (void)stop;
- (void)test;
@end
