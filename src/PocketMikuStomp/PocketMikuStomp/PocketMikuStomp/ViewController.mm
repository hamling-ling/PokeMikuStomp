//
//  ViewController.m
//  PocketMikuStomp
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import "ViewController.h"
#import "PokeMikuStompLib.h"

@interface ViewController() {
    PokeMikuStompLib* _miku;
}
@property (nonatomic, readwrite, assign) bool isDeviceReady;
@property (nonatomic, readwrite, assign) bool isPokeMikuReady;
@property (nonatomic, readwrite, strong) NSString* inlineError;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    self.isDeviceReady = NO;
    self.isPokeMikuReady = NO;
    
    _miku = [[PokeMikuStompLib alloc] init];
    auto err = [_miku setup];
    if(kPokeMikuStompLibNoError != err) {
        [self alertForPokeMikuError:err];
        return;
    }
    
    self.isPokeMikuReady = YES;
    
    NSArray* devices = [_miku devices];
    self.arrayController.content = devices;
    if(devices.count > 0) {
        self.selectedIndex = 0;
        auto err = [_miku selectDeviceWithId: self.selectedIndex ];
        if(kPokeMikuStompLibNoError != err) {
            [self alertForPokeMikuError:err];
            return;
        }
        self.isDeviceReady = YES;
    }
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (IBAction)startButtonPressed:(id)sender {
    [_miku start];
}

- (IBAction)stopButtonPressed:(id)sender {
    [_miku stop];
}

- (IBAction)deviceSelected:(id)sender {
    NSPopUpButton* popbutton = (NSPopUpButton*)sender;
    auto err = [_miku selectDeviceWithId:(int)popbutton.indexOfSelectedItem];
    if(kPokeMikuStompLibNoError != err) {
        [self alertForPokeMikuError:err];
        return;
    }
    self.isDeviceReady = YES;
}

- (void)alertForPokeMikuError:(const PokeMikuStompLibError)err {
    NSString* msg = [self pokeMikuErrorToString:err];
    if([msg isEqualToString:@""]) {
        return;
    }
    
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:msg];
    NSString* infoText = NSLocalizedString(@"ConnectAndReboot", nil);
    [alert setInformativeText:infoText];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert runModal];
    
    self.inlineError = [NSString stringWithFormat:@"%@. %@", msg, infoText];
}

- (NSString*)pokeMikuErrorToString:(const PokeMikuStompLibError)err {
    NSString* key = @"";
    switch(err) {
        case kPokeMikuStompLibErrorPokeMikuNotFound:
            key = @"PokeMikuNotFound";
            break;
        case kPokeMikuStompLibErrorNoCaptureDevice:
            key = @"NoCaptureDevice";
            break;
        case kPokeMikuStompLibErrorInternalError:
            key = @"InternalError";
            break;
        default:
            break;
    }
    if([key isEqualToString:@""]) {
        return @"";
    }
    
    return NSLocalizedString(key,nil);
}

@end
