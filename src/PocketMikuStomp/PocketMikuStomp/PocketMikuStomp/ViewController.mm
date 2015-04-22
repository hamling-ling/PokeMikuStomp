//
//  ViewController.m
//  PocketMikuStomp
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import "ViewController.h"
#import "PokeMikuStompLib.h"

static NSString* const kNoteStringProp = @"noteString";
static NSString* const kInputLevelProp = @"inputLevel";
static NSString* const kPronoucingStringProp = @"pronouncingString";

@interface ViewController() {
    PokeMikuStompLib* _miku;
}
@property (nonatomic, readwrite, assign) bool isDeviceReady;
@property (nonatomic, readwrite, assign) bool isPokeMikuReady;
@property (nonatomic, readwrite, strong) NSString* inlineError;
@property (nonatomic, readwrite, strong) NSString* noteString;
@property (nonatomic, readwrite, strong) NSString* pronouncingString;
@property (nonatomic, readwrite, assign) NSInteger level;
@property (nonatomic, readwrite, assign) NSInteger OffToOnThreshold;
@property (nonatomic, readwrite, assign) NSInteger OnToOffThreshold;
@property (weak) IBOutlet NSMatrix *phraseRadioGroup;
@property (nonatomic, readwrite, assign) bool isPhraseEditEnabled;

@end

@implementation ViewController

#pragma mark Livfe cycles

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    self.isDeviceReady = NO;
    self.isPokeMikuReady = NO;
    self.level = 0;

    _miku = [[PokeMikuStompLib alloc] init];
    auto err = [_miku setup];
    if(kPokeMikuStompLibNoError != err) {
        [self alertForPokeMikuError:err];
        return;
    }
    
    self.OffToOnThreshold = _miku.OffToOnThreshold;
    self.OnToOffThreshold = _miku.OnToOffThreshold;
    self.isPokeMikuReady = YES;
    [self.phraseRadioGroup selectCellAtRow:0 column:0];
    self.edittingPhrase = @"どれみ";
    
    self.phrase1 = @"さくらさくら";
    NSCell* cell1 = (NSCell*)self.phraseRadioGroup.cells[1];
    [self setString:self.phrase1 toCell:cell1];
    
    self.phrase2 = @"よまえやよまえ";
    NSCell* cell2 = (NSCell*)self.phraseRadioGroup.cells[2];
    [self setString:self.phrase2 toCell:cell2];
    
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
    
    [_miku addObserver:self forKeyPath:kNoteStringProp options:NSKeyValueObservingOptionNew context:nil];
    [_miku addObserver:self forKeyPath:kInputLevelProp options:NSKeyValueObservingOptionNew context:nil];
    [_miku addObserver:self forKeyPath:kPronoucingStringProp options:NSKeyValueObservingOptionNew context:nil];
}

- (void)viewWillDisappear {
    [super viewWillDisappear];
    
    [_miku removeObserver:self forKeyPath:kNoteStringProp];
    [_miku removeObserver:self forKeyPath:kInputLevelProp];
    [_miku removeObserver:self forKeyPath:kPronoucingStringProp];
}

#pragma mark KVO

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:kNoteStringProp]) {
        self.noteString = _miku.noteString;
    } else if([keyPath isEqualToString:kInputLevelProp]) {
        self.level = _miku.inputLevel;
    } else if([keyPath isEqualToString:kPronoucingStringProp]) {
        self.pronouncingString = _miku.pronouncingString;
    }
}

#pragma mark Actions

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

- (IBAction)testButtonPressed:(id)sender {
    [_miku test];
}

- (IBAction)offOnSliderChanged:(id)sender {
    NSSlider* sld = (NSSlider*)sender;
    int onOffValue = _miku.OnToOffThreshold;
    if (sld.intValue < onOffValue) {
        // cancel change
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                       ^() {[sld setIntValue:onOffValue];}
                       );
    } else {
        _miku.OffToOnThreshold = sld.intValue;
    }
}

- (IBAction)onOffSliderChanged:(id)sender {
    NSSlider* sld = (NSSlider*)sender;
    int offOnValue = _miku.OffToOnThreshold;
    if (sld.intValue > _miku.OffToOnThreshold) {
        // cancel change
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                       ^() {[sld setIntValue:offOnValue];}
                       );
    } else {
        _miku.OnToOffThreshold = sld.intValue;
    }
}

- (IBAction)phraseDoremiSelected:(id)sender {
    self.isPhraseEditEnabled = NO;
    self.edittingPhrase = @"どれみ";
    self.phrasedString = _edittingPhrase;
    _miku.voiceMode = kPokeMikuStompLibVoiceModeDoremi;
}

- (IBAction)phrase1Selected:(id)sender {
    _miku.voiceMode = kPokeMikuStompLibVoiceModeUserPhrase;
    _miku.currentPhrase = self.phrase1;
    self.isPhraseEditEnabled = YES;
    self.edittingPhrase = _miku.currentPhrase;
    self.phrasedString = _edittingPhrase;
}

- (IBAction)phrase2Selected:(id)sender {
    _miku.voiceMode = kPokeMikuStompLibVoiceModeUserPhrase;
    _miku.currentPhrase = self.phrase2;
    self.isPhraseEditEnabled = YES;
    self.edittingPhrase = _miku.currentPhrase;
    self.phrasedString = _edittingPhrase;
}

- (IBAction)phraseTextFieldInput:(id)sender {
    NSTextField* tf = (NSTextField*)sender;
    [self handleTextFieldUpdate:tf];
}

#pragma mark <NSTextFieldDelegate>

- (void)controlTextDidChange:(NSNotification *)notification {
    NSTextField* tf = (NSTextField*)notification.object;
    [self handleTextFieldUpdate:tf];
}

#pragma mark Private methods

- (void)handleTextFieldUpdate:(NSTextField*)tf {
    _miku.currentPhrase = tf.stringValue;
    self.edittingPhrase = _miku.currentPhrase;
    self.phrasedString = _edittingPhrase;
    
    NSCell* cell = (NSCell*)self.phraseRadioGroup.selectedCell;
    [self setString:_edittingPhrase toCell:cell];
    if(self.phraseRadioGroup.selectedRow == 1) {
        self.phrase1 = _edittingPhrase;
    } else if(self.phraseRadioGroup.selectedRow == 2) {
        self.phrase2 = _edittingPhrase;
    }
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

- (void)setString:(NSString*)str toCell:(NSCell*)cell {
    cell.title = [NSString stringWithFormat:@"%@%@", [str substringToIndex:4], @"..."];
}

@end
