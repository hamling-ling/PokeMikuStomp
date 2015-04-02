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
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    _miku = [[PokeMikuStompLib alloc] init];
    //[_miku test];
    [_miku setup];
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

@end
