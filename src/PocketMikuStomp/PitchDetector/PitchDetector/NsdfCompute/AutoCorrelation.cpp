#include "AutoCorrelation.h"
#include <fftw3.h>
#include <memory>

AutoCorrelation::AutoCorrelation(int windowSize)
	: kWinSize(windowSize), kFftSize(windowSize*2), _src(NULL), _fft(NULL), _powspec(NULL)
{
	_src = static_cast<float*>(fftwf_malloc(sizeof(float) * kFftSize));
	_fft = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * kFftSize));
	_powspec = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * kFftSize));
	_ifft = static_cast<float*>(fftwf_malloc(sizeof(float) * kFftSize));
}

AutoCorrelation::~AutoCorrelation()
{
	fftwf_free(_src);
	fftwf_free(_fft);
	fftwf_free(_powspec);
	fftwf_free(_ifft);
}

void AutoCorrelation::Compute(float* x, float *corr)
{
	memset(_src, 0, kFftSize);
	memset(_fft, 0, sizeof(fftwf_complex) * kFftSize);
	memset(_powspec, 0, sizeof(fftwf_complex) * kFftSize);

	// padding 0 half of src
	memcpy(_src, x, sizeof(float)*kWinSize);

	// fft
	fftwf_plan plan_fft = fftwf_plan_dft_r2c_1d(kFftSize, _src, _fft, FFTW_ESTIMATE);
	fftwf_execute(plan_fft);
	fftwf_destroy_plan(plan_fft);
	
	// power spectrum
	for (int i = 0; i < kFftSize; i++) {
		float powered = _fft[i][0] * _fft[i][0] + _fft[i][1] * _fft[i][1];
		_powspec[i][0] = powered / kFftSize;
		_powspec[i][1] = 0.0f;
	};
	
	// ifft
	fftwf_plan plan_ifft = fftwf_plan_dft_c2r_1d(kFftSize, _powspec, _ifft, FFTW_ESTIMATE);
	fftwf_execute(plan_ifft);
	fftwf_destroy_plan(plan_ifft);

	memcpy(corr, _ifft, sizeof(float)*kWinSize);
}

int AutoCorrelation::WindowSize()
{
	return kWinSize;
}
