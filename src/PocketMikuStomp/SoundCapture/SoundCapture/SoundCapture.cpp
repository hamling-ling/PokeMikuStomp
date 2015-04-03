#include "SoundCapture.h"

#include <algorithm>

//#define OUTPUT_TO_FILE

#ifdef OUTPUT_TO_FILE
#include <iostream>// debug
#include <fstream>
#endif

#include "CaptureDevice.h"

using namespace std;

SoundCapture::SoundCapture(int sampleRate, int sampleNum)
: _sampleBuf(NULL), _device(make_shared<CaptureDevice>(sampleRate, sampleNum))
{
}

SoundCapture::~SoundCapture()
{
	Stop();
	if(_device) {
		_device->DestroyDevice();
	}
	delete(_sampleBuf);
}

bool SoundCapture::Initialize(SoundCaptureCallback_t callback, void* user)
{
	if(!_sampleBuf) {
		return true;
	}
	
	_callback = callback;
	_sampleBuf = new int16_t[_device->SampleNumber()];
	_user = user;

	if(!_sampleBuf) {
		return false;
	}
	
	return true;
}

SoundCaptureError SoundCapture::Start()
{
	if(!_sampleBuf) {
		return SoundCaptureErrorNotInitialized;
	}
	
	if(_device->SelectedDevice() == -1) {
		return SoundCaptureErrorNoDevice;
	}
	
	ServiceStart();

	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::Stop()
{
	ServiceStop();
	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::GetDevices(std::vector<std::string>& vec)
{
	_device->GetDevices(vec);
	return SoundCaptureErrorNoError;
}

int SoundCapture::Level()
{
	std::lock_guard<std::recursive_mutex> lock(_dataMutex);
	return static_cast<int>(_level / 32768.0f * 100);
}

SoundCaptureError SoundCapture::SelectDevice(int index)
{
	if(IsRunning()) {
		return SoundCaptureErrorAlreadyRunning;
	}
	
	if(_device->SelectedDevice() != -1) {
		return SoundCaptureErrorDeviceExist;
	}
	
	CaptureDeviceError err = _device->CreateDevice(index);
	if(CaptureDeviceErrorDeviceExist == err) {
		return SoundCaptureErrorNoDevice;
	}
	
	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::DeselectDevice()
{
	if(IsRunning()) {
		return SoundCaptureErrorAlreadyRunning;
	}
	
	if(_device->SelectedDevice() == -1) {
		return SoundCaptureErrorNoError;
	}
	
	CaptureDeviceError err = _device->DestroyDevice();
	if(CaptureDeviceErrorDeviceExist == err) {
		return SoundCaptureErrorInternal;
	}
	
	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::GetBuffer(float* out)
{
	if (out) {
		std::lock_guard<std::recursive_mutex> lock(_dataMutex);
		int N = _device->SampleNumber();
		for (int i = 0; i < N; i++) {
			out[i] = _sampleBuf[i] / 32768.0f;
		}
	}
	return SoundCaptureErrorNoError;
}

void SoundCapture::ServiceProc()
{
	const int N = _device->SampleNumber();
	ALshort* data = static_cast<int16_t*>(_sampleBuf);

	if(!_sampleBuf) {
		NotifyCaptureError(SoundCaptureErrorNotInitialized);
		ProcFinished();
		return;
	}
	
	if(_device->SelectedDevice() == -1) {
		NotifyCaptureError(SoundCaptureErrorNoDevice);
		ProcFinished();
		return;
	}
	
	_device->CaptureStart();

	CaptureDeviceError err = CaptureDeviceErrorNoError;
	while (!IsStopRequested())
	{
		{
			std::lock_guard<std::recursive_mutex> lock(_dataMutex);
			
			// Read the captured audio
			memset(data, 0, sizeof(ALshort) * N);
			CaptureDeviceError err = _device->Sample(data);
			if(CaptureDeviceErrorTooEarly == err) {
				continue;
			}
			
			if(CaptureDeviceErrorNoError == err) {
				//  Process/filter captured data
				ProcessData(static_cast<int16_t*>(data), N);
			}
		}

		if(!_callback) {
			continue;
		}
		
		SoundCaptureNotification note;
		if(CaptureDeviceErrorNoError == err) {
			note.type = SoundCaptureNotificationTypeCaptured;
			note.user = _user;
			note.err = SoundCaptureErrorNoError;
			_callback(this, note);
		} else {
			NotifyCaptureError(SoundCaptureErrorInternal);
			break;
		}
	}

	_device->CaptureStop();
}

void SoundCapture::ProcessData(int16_t *data, int dataNum)
{
#ifdef OUTPUT_TO_FILE
	std::ofstream outfile("new.txt", std::ofstream::trunc);
#endif

	int16_t maxValue = 0;
	for (int i = 0; i<dataNum; i++) {
#ifdef OUTPUT_TO_FILE
		outfile << int(data[i]) << endl;
#endif
		maxValue = max(data[i], maxValue);
	}

	_level = maxValue;
}

void SoundCapture::NotifyCaptureError(SoundCaptureError err)
{
	SoundCaptureNotification note;
	note.type = SoundCaptureNotificationTypeCaptureError;
	note.err = err;
	if(_callback) {
		_callback(this, note);
	}
}
