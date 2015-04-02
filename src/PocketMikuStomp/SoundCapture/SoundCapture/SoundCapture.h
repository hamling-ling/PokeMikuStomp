#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <thread>
#include <mutex>

typedef enum _SoundCaptureError {
	SoundCaptureErrorNoError,
	SoundCaptureErrorAlreadyRunning,
	SoundCaptureErrorNoDevice,
	SoundCaptureErrorInternal,
} SoundCaptureError;

typedef enum _SoundCaptureNotificationType {
	SoundCaptureNotificationTypeCaptured,
} SoundCaptureNotificationType;

struct SoundCaptureNotification {
	SoundCaptureNotificationType type;
	void* user;
};

class SoundCapture;
typedef std::function < void(SoundCapture*, SoundCaptureNotification)> SoundCaptureCallback_t;

class SoundCapture
{
public:
	SoundCapture(int sampleRate, int sampleNum);
	virtual ~SoundCapture();

	bool Initialize(SoundCaptureCallback_t callback, void* user);
	SoundCaptureError Start();
	SoundCaptureError Stop();
	SoundCaptureError GetDevices(std::vector<std::string>& vec);
	SoundCaptureError SelectDevice(int index);

	/**
	 *	Get recording signal level.
	 */
	int Level();

	/**
	 *	
	 */
	SoundCaptureError GetBuffer(float* out);

private:
	const int _sampleRate;
	const int _sampleNum;
	int16_t* _sampleBuf;
	int _selectedDeviceIndex;
	std::thread _thread;
	bool _isRunning;
	bool _stopRunning;
	std::recursive_mutex _apiMutex;
	std::recursive_mutex _dataMutex;
	uint16_t _level;
	void* _user;
	SoundCaptureCallback_t _callback;

	void CaptureLoop();
	void ProcessData(int16_t* data, int dataNum);
};

