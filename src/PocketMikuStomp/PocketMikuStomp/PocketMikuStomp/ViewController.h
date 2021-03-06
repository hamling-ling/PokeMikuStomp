//
//  ViewController.h
//  PocketMikuStomp
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController <NSTextFieldDelegate>
@property (nonatomic, readonly, assign) bool isPokeMikuReady;
@property (nonatomic, readonly, assign) bool isDeviceReady;
@property (strong) IBOutlet NSArrayController *arrayController;
@property (nonatomic, assign) int selectedIndex;
@property (weak) IBOutlet NSPopUpButton *deviceSelection;
@property (nonatomic, readonly, strong) NSString* inlineError;
@property (nonatomic, readonly, strong) NSString* noteString;
@property (nonatomic, readonly, strong) NSString* pronouncingString;
@property (nonatomic, readonly, assign) NSInteger level;
@property (nonatomic, readonly, assign) NSInteger OffToOnThreshold;
@property (nonatomic, readonly, assign) NSInteger OnToOffThreshold;
@property (nonatomic, readwrite, strong) NSString* phrase1;
@property (nonatomic, readwrite, strong) NSString* phrase2;
@property (nonatomic, readwrite, strong) NSString* edittingPhrase;
@property (nonatomic, readwrite, strong) NSString* phrasedString;

@end

