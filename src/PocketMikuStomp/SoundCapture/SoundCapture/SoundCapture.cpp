#include "SoundCapture.h"
#if WIN32
#include <al.h>
#include <alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include <algorithm>

//#define OUTPUT_TO_FILE

#ifdef OUTPUT_TO_FILE
#include <iostream>// debug
#include <fstream>
#endif

using namespace std;

SoundCapture::SoundCapture(int sampleRate, int sampleNum)
: _sampleRate(sampleRate), _sampleNum(sampleNum), _sampleBuf(NULL), _selectedDeviceIndex(-1), _isRunning(false), _stopRunning(false)
{
}

SoundCapture::~SoundCapture()
{
	delete(_sampleBuf);
}

bool SoundCapture::Initialize(SoundCaptureCallback_t callback, void* user)
{
	_callback = callback;
	_sampleBuf = new int16_t[_sampleNum];
	_user = user;

	return true;
}

SoundCaptureError SoundCapture::Start()
{
	std::lock_guard<std::recursive_mutex> lock(_apiMutex);

	if (_isRunning) {
		return SoundCaptureErrorAlreadyRunning;
	}

	_thread = thread(&SoundCapture::CaptureLoop, this);
	_isRunning = true;

	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::Stop()
{
	std::lock_guard<std::recursive_mutex> lock(_apiMutex);
	if (_isRunning) {
		_stopRunning = true;
	}

	if (_thread.joinable()) {
		_thread.join();
	}

	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::GetDevices(std::vector<std::string>& vec)
{
	return SoundCaptureErrorNoError;
}

int SoundCapture::Level()
{
	std::lock_guard<std::recursive_mutex> lock(_dataMutex);
	return static_cast<int>(_level / 32768.0f * 100);
}

SoundCaptureError SoundCapture::SelectDevice(int index)
{
	return SoundCaptureErrorNoError;
}

SoundCaptureError SoundCapture::GetBuffer(float* out)
{
	if (out) {
		std::lock_guard<std::recursive_mutex> lock(_dataMutex);
		for (int i = 0; i < _sampleNum; i++) {
			out[i] = _sampleBuf[i] / 32768.0f;
		}
	}
	return SoundCaptureErrorNoError;
}

void SoundCapture::CaptureLoop()
{
	ALCdevice *dev = NULL;
	ALCcontext *ctx = NULL;
	ALshort* data = static_cast<int16_t*>(_sampleBuf);

	/* If you don't need 3D spatialization, this should help processing time */
	alDistanceModel(AL_NONE);
	dev = alcCaptureOpenDevice(NULL, _sampleRate, AL_FORMAT_MONO16, sizeof(ALshort)*_sampleNum);

	ctx = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(ctx);

	/* Start capture, and enter the audio loop */
	alcCaptureStart(dev);    //starts ring buffer

	while (!_stopRunning)
	{
		/* Check how much audio data has been captured (note that 'val' is the
		* number of frames, not bytes) */
		ALint capturedFrameNum;
		alcGetIntegerv(dev, ALC_CAPTURE_SAMPLES, 1, &capturedFrameNum);

		if (capturedFrameNum <= _sampleNum) {
			continue;
		}

		{
			std::lock_guard<std::recursive_mutex> lock(_dataMutex);
			/* Read the captured audio */
			memset(data, 0, sizeof(ALshort) * _sampleNum);
			alcCaptureSamples(dev, data, _sampleNum);

			//***** Process/filter captured data here *****//
			ProcessData(static_cast<int16_t*>(data), _sampleNum);
		}

		if (_callback) {
			SoundCaptureNotification note;
			note.type = SoundCaptureNotificationTypeCaptured;
			note.user = _user;
			_callback(this, note);
		}
	}

	/* Shutdown and cleanup */
	alcCaptureStop(dev);
	alcCaptureCloseDevice(dev);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);

	_isRunning = false;
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
