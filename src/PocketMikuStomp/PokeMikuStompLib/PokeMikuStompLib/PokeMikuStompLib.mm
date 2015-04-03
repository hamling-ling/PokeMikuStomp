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

using namespace std;

struct AppData {
    shared_ptr<PitchDetector> det;
    float* buf;
    void* context;
    void* miku;
};

static const shared_ptr<PitchDetector> nullDet;
static const shared_ptr<SoundCapture> nullCap;

static void SoundCapEvent(SoundCapture* sc, SoundCaptureNotification note)
{
    @autoreleasepool {
        if(SoundCaptureNotificationTypeCaptured == note.type) {
            int level = sc->Level();
            if (level < 20) {
                return;
            }
            
            AppData* appData = static_cast<AppData*>(note.user);
            sc->GetBuffer(appData->buf);
            
            if (appData->det->Detect(appData->buf)) {
                PitchInfo pitch;
                appData->det->GetPiatch(pitch);
                
                PokeMikuStompLib* lib = (__bridge id)appData->context;
                PMMiku* miku = (__bridge id)appData->miku;
                
                [miku noteOnWithKey:pitch.midi velocity:100 pronunciation:@"にゃ"];
                //cout << "level:" << level << ", pitch:" << pitch.noteStr << endl;
                NSLog(@"level=%d, note=%d", level, pitch.midi);
            }
        }
    }
}

@interface PokeMikuStompLib() {
    PMMiku* _miku;
    shared_ptr<PitchDetector> _det;
    shared_ptr<SoundCapture> _cap;
    AppData _app;
}
@end

@implementation PokeMikuStompLib

- (id)init {
    self = [super init];
    if(self) {
        _det = nullDet;
        _cap = nullCap;
        _app.det = nullDet;
        _app.context = NULL;
        _app.buf = NULL;
        _app.miku = NULL;
    }
    return self;
}

- (void)dealloc {
    [self teardown];
}

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

@end
