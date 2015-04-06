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
}
@property (atomic, readwrite, assign) int inputLevel;
@property (atomic, readwrite, assign) int midiNote;
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
    }
    return self;
}

- (void)dealloc {
    [self teardown];
}

#pragma Public Interface

- (void)test {
    PMMiku *miku = [[PMMiku alloc] init];
    
    NSArray* arr = @[@"あ", @"い", @"う", @"え", @"お"];
    for(int i = 0; i < arr.count; i++) {
        [miku noteOnWithKey:64+i velocity:100 pronunciation:arr[i]];
        sleep(1);
    }
    
    [miku noteOff];
}

- (PokeMikuStompLibError)setup {
    const int samplingRate = 22050;
    const int samplingSize = 1024;
	
	if(_miku) {
		// for debug
		//return kPokeMikuStompLibNoError;
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
        //return kPokeMikuStompLibErrorPokeMikuNotFound;
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
    delete(_app.buf);
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
        //return kPokeMikuStompLibErrorPokeMikuNotFound;
    }
    
    if(_cap) {
        SoundCaptureError err = _cap->SelectDevice(deviceId);
        return [self PokeMikuErrorFromCaptureError:err];
    }
    return kPokeMikuStompLibErrorNotInitialized;
}

- (PokeMikuStompLibError)start {

    if(!_miku) {
        //return kPokeMikuStompLibErrorPokeMikuNotFound;
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

#pragma Event Handlers

- (void) captureEventNotifiedFrom:(SoundCapture*)sc notification:(SoundCaptureNotification&)note {
    int level = sc->Level();
    if(_inputLevel != level) {
        self.inputLevel = level;
    }
    
    if(!sc) {
        [_miku noteOff];
        self.midiNote = -1;
        return;
    }
    
    if(!_app.buf) {
        [_miku noteOff];
        return;
    }
    
    if(!_app.det) {
        [_miku noteOff];
        return;
    }
	
	
	shared_ptr<StopWatch> _sw_getbuf;
	shared_ptr<StopWatch> _sw_detect;
	_sw_getbuf = make_shared<StopWatch>("getbuf");

	
	for(int i = 0; i < 1024; i++) {
		sc->GetBuffer(_app.buf);
		_sw_getbuf->CountUp();
	}
	NSLog(@"getbuf %llu", _sw_getbuf->Time());
	
	_sw_detect = make_shared<StopWatch>("detect");
	for(int i = 0; i < 1024; i++) {
		if(_det->Detect(_app.buf)) {
			// do nothing when detection failed
		}
		_sw_detect->CountUp();
	}
	NSLog(@"detect %llu", _sw_detect->Time());
	
    PitchInfo pitch;
    _det->GetPiatch(pitch);
    if (level < self.levelThreshold) {
        [_miku noteOff];
        self.midiNote = -1;
        NSLog(@"level=%d, note=off", level);
        return;
    }
    
    if(_midiNote != pitch.midi) {
        [_miku noteOnWithKey:pitch.midi velocity:100 pronunciation:@"にゃ"];
        self.midiNote = pitch.midi;
        NSLog(@"level=%d, note=%d", level, pitch.midi);
    }
}

@end
