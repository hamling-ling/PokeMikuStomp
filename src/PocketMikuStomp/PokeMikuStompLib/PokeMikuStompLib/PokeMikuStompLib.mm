//
//  PokeMikuStompLib.m
//  PokeMikuStompLib
//
//  Created by Nobuhiro Kuroiwa on 2015/04/02.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#import "PokeMikuStompLib.h"
#include <memory>
#include <vector>
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
        
    }
    return self;
}

- (void)dealloc {
    delete(_app.buf);
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

- (int)setup {
    const int samplingRate = 22050;
    const int samplingSize = 1024;
	
	if(_miku) {
		return -1;
	}
	
    _miku = [[PMMiku alloc] init];
    _det = make_shared<PitchDetector>(samplingRate, samplingSize);
    if(!_det->Initialize()) {
        return -1;
    }
    
    _cap = make_shared<SoundCapture>(samplingRate, samplingSize);
    _app.det = _det;
    _app.context = (__bridge void*)self;
    _app.buf = new float[samplingSize];
    _app.miku = (__bridge void*)_miku;
    
    if(!_cap->Initialize(SoundCapEvent, static_cast<void*>(&_app))) {
        return -1;
    }
    
    return 0;
}

- (int)teardown {
	return 0;
}

- (NSArray*)devices {
	vector<string> vec;
	_cap->GetDevices(vec);
	
	NSMutableArray* m = [NSMutableArray array];
	for(auto d:vec) {
		NSString *deviceName = [NSString stringWithUTF8String:d->c_str()];
		[m addObject:deviceName];
	}
	
	return m;
}

- (int)selectDeviceWithId:(int)deviceId {
	
}

- (int)start {

    _cap->Start();
    
    return 0;
}

- (void)stop {
    _cap->Stop();
    [_miku noteOff];
}

@end
