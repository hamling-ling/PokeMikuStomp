//
//  PokeMikuStompLib.m
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import "PokeMikuStompLib.h"
#include <memory>
#include <PitchDetector.h>
#include <SoundCapture.h>
#include <PMMiku.h>
#include "StopWatch.h"

using namespace std;

struct AppData {
    shared_ptr<PitchDetector> det;
    float* buf;
    void* context;
    void* miku;
};

static const shared_ptr<PitchDetector> nullDet;
static const shared_ptr<SoundCapture> nullCap;

@interface PokeMikuStompLib() {
    PMMiku* _miku;
    shared_ptr<PitchDetector> _det;
    shared_ptr<SoundCapture> _cap;
    AppData _app;
    NSArray* _pronounciations;
    NSUInteger _currentProIndex;
}

@property (atomic, readwrite, assign) int inputLevel;
@property (atomic, readwrite, assign) int midiNote;
@property (atomic, readwrite, strong) NSString* noteString;

- (void) captureEventNotifiedFrom:(SoundCapture*)sc notification:(SoundCaptureNotification&)note;
@end

static void SoundCapEvent(SoundCapture* sc, SoundCaptureNotification note)
{
    @autoreleasepool {
        AppData* appData = static_cast<AppData*>(note.user);
        if(!appData) {
            return;
        }
        
        PokeMikuStompLib* lib = (__bridge id)appData->context;
        if(!lib) {
            return;
        }
        
        if(SoundCaptureNotificationTypeCaptured == note.type) {
            [lib captureEventNotifiedFrom:sc notification:note];
        }
    }
}

@implementation PokeMikuStompLib

#pragma mark Life cycles

- (id)init {
    self = [super init];
    if(self) {
        _det = nullDet;
        _cap = nullCap;
        _app.det = nullDet;
        _app.context = NULL;
        _app.buf = NULL;
        _app.miku = NULL;
        _levelThreshold = kDefaultLevelThreshold;
        
        _pronounciations = @[@"た", @"ら", @"り", @"ら"];
        _currentProIndex = 0;
    }
    return self;
}

- (void)dealloc {
    [self teardown];
}

#pragma Public Interface

- (void)test {
    if(!_miku) {
        return;
    }
    
    NSArray* arr = @[@"ど", @"み", @"そ", @"ど"];
    [_miku noteOnWithKey:48+12 velocity:100 pronunciation:arr[0]];
    usleep(0.25 * 1000 * 1000);
    
    [_miku noteOnWithKey:52+12 velocity:100 pronunciation:arr[1]];
    usleep(0.25 * 1000 * 1000);
    
    [_miku noteOnWithKey:55+12 velocity:100 pronunciation:arr[2]];
    usleep(0.25 * 1000 * 1000);
    
    [_miku noteOnWithKey:60+12 velocity:100 pronunciation:arr[3]];
    usleep(0.25 * 1000 * 1000);
    
    [_miku noteOff];
}

- (PokeMikuStompLibError)setup {
    const int samplingRate = 22050;
    const int samplingSize = 1024;
	
	if(_miku) {
		return kPokeMikuStompLibNoError;
	}
	
    shared_ptr<PitchDetector> det = make_shared<PitchDetector>(samplingRate, samplingSize);
    if(!det->Initialize()) {
        return kPokeMikuStompLibErrorInternalError;
    }
   
    shared_ptr<SoundCapture> cap = make_shared<SoundCapture>(samplingRate, samplingSize);
    if(!cap->Initialize(SoundCapEvent, static_cast<void*>(&_app))) {
        return kPokeMikuStompLibErrorInternalError;
    }
    
    _miku = [[PMMiku alloc] init];
    if(!_miku) {
        return kPokeMikuStompLibErrorPokeMikuNotFound;
    }
    
    _det = det;
    _cap = cap;
    _app.det = _det;
    _app.context = (__bridge void*)self;
    _app.buf = new float[samplingSize];
    _app.miku = (__bridge void*)_miku;
    
    return kPokeMikuStompLibNoError;
}

- (void)teardown {
    
    if(!_miku) {
        return;
    }
    
    [self stop];
    
    _miku = nil;
    _cap->DeselectDevice();
    _cap = nullCap;
    _det = nullDet;
    delete[] (_app.buf);
    _app.buf = NULL;
    _app.context = NULL;
    _app.miku = NULL;
    _app.det = nullDet;
}

- (NSArray*)devices {
	vector<string> vec;
    
    if(!_cap) {
        return @[];
    }
    
	_cap->GetDevices(vec);
	
	NSMutableArray* m = [NSMutableArray array];
	for(auto d : vec) {
		NSString *deviceName = [NSString stringWithUTF8String:d.c_str()];
		[m addObject:deviceName];
	}
	
	return m;
}

- (PokeMikuStompLibError)selectDeviceWithId:(int)deviceId {
    
    if(!_miku) {
        return kPokeMikuStompLibErrorPokeMikuNotFound;
    }
    
    if(_cap) {
        SoundCaptureError err = _cap->SelectDevice(deviceId);
        return [self PokeMikuErrorFromCaptureError:err];
    }
    return kPokeMikuStompLibErrorNotInitialized;
}

- (PokeMikuStompLibError)start {

    if(!_miku) {
        return kPokeMikuStompLibErrorPokeMikuNotFound;
    }
    
    if(!_cap) {
        return kPokeMikuStompLibErrorNotInitialized;
    }
    
    if(_cap->SelectedDevice() == -1) {
        return kPokeMikuStompLibErrorNoCaptureDevice;
    }
    
    auto err = _cap->Start();
    if(SoundCaptureErrorNoError != err) {
        return [self PokeMikuErrorFromCaptureError:err];
    }
    return kPokeMikuStompLibNoError;
}

- (void)stop {
    if(!_cap) {
        return;
    }
    
    _cap->Stop();
    [_miku noteOff];
}

#pragma mark Private methods

- (PokeMikuStompLibError)PokeMikuErrorFromCaptureError:(const SoundCaptureError) capErr {
    PokeMikuStompLibError stompErr = kPokeMikuStompLibNoError;
    switch(capErr) {
        case SoundCaptureErrorNoError :
            break;
        case SoundCaptureErrorAlreadyRunning:
            stompErr = kPokeMikuStompLibErrorCaptureRunning;
            break;
        case SoundCaptureErrorNotInitialized:
            stompErr = kPokeMikuStompLibErrorCaptureNotInitialized;
            break;
        case SoundCaptureErrorNoDevice:
            stompErr = kPokeMikuStompLibErrorNoCaptureDevice;
            break;
        case SoundCaptureErrorDeviceExist:
            stompErr = kPokeMikuStompLibErrorCaptureDeviceExist;
            break;
        case SoundCaptureErrorInternal:
        default:
            stompErr = kPokeMikuStompLibErrorInternalError;
            break;
    }
    return stompErr;
    
}

- (bool)noteOff {
    if(_midiNote == -1) {
        return NO;
    }
    
    [_miku noteOff];
    self.midiNote = -1;
    self.noteString = @"";
    
    return YES;
}

#pragma Event Handlers

- (void) captureEventNotifiedFrom:(SoundCapture*)sc notification:(SoundCaptureNotification&)note {
    int level = sc->Level();
    if(_inputLevel != level) {
        self.inputLevel = level;
    }
    
    if(!sc) {
        [self noteOff];
        return;
    }
    
    if(!_app.buf) {
        [self noteOff];
        return;
    }
    
    if(!_app.det) {
        [self noteOff];
        return;
    }
	
	
    if(_det->Detect(sc->GetRawBufferPointer())) {
        // do nothing when detection failed
    }
	
    PitchInfo pitch;
    _det->GetPiatch(pitch);
    if (level < self.levelThreshold) {
        if([self noteOff]) {
            NSLog(@"level=%d, note=off", level);
        }
        return;
    }
    
    if(_midiNote != pitch.midi) {
        NSString* pro = _pronounciations[_currentProIndex];
        _currentProIndex++;
        _currentProIndex %= _pronounciations.count;
        [_miku noteOnWithKey:pitch.midi velocity:level pronunciation:pro];

        self.midiNote = pitch.midi;
        NSLog(@"level=%d, note=%d", level, pitch.midi);
        self.noteString = [NSString stringWithUTF8String:pitch.noteStr];
    }
}

@end
