//
//  ViewController.h
//  PocketMikuStomp
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController
@property (nonatomic, readonly, assign) bool isPokeMikuReady;
@property (nonatomic, readonly, assign) bool isDeviceReady;
@property (strong) IBOutlet NSArrayController *arrayController;
@property (nonatomic, assign) int selectedIndex;
@property (weak) IBOutlet NSPopUpButton *deviceSelection;
@property (nonatomic, readonly, strong) NSString* inlineError;

@end

