#pragma once

#include <fftw3.h>

class AutoCorrelation
{
public:
	AutoCorrelation(int windowSize);
	virtual ~AutoCorrelation();
	void Compute(float* x, float *corr);
	int WindowSize();
private:
	const int kWinSize;
	const int kFftSize;
	float* _src;
	fftwf_complex* _fft;
	fftwf_complex* _powspec;
	float* _ifft;
};

