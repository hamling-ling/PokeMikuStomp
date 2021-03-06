//
//  CaptureDevice.h
//  SoundCapture
//
//  Created by Nobuhiro Kuroiwa on 2015/04/03.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __SoundCapture__CaptureDevice__
#define __SoundCapture__CaptureDevice__

#include <vector>
#include <string>

#if WIN32
#include <al.h>
#include <alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

typedef enum _CaptureDeviceError {
	CaptureDeviceErrorNoError,
	CaptureDeviceErrorNoDevice,
	CaptureDeviceErrorDeviceExist,
	CaptureDeviceErrorTooEarly,
	CaptureDeviceErrorInternal,
} CaptureDeviceError;

class CaptureDevice {
public:
	CaptureDevice(int sampleRate, int sampleNum);
	virtual ~CaptureDevice();
	
	int SelectedDevice();
	CaptureDeviceError GetDevices(std::vector<std::string>& vec);
	CaptureDeviceError CreateDevice(int index);
	CaptureDeviceError DestroyDevice();
	CaptureDeviceError CaptureStart();
	CaptureDeviceError CaptureStop();
	
	int SamplingRate();
	int SampleNumber();
	CaptureDeviceError Sample(ALshort* data);
	
private:
	const int _sampleRate;
	const int _sampleNum;
	int _selectedDeviceIndex;
	ALCdevice *_dev;
	ALCcontext *_ctx;
};

#endif /* defined(__SoundCapture__CaptureDevice__) */
