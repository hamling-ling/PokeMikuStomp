//
//  AutoCorrelationW.cpp
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/08.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "AutoCorrelationW.h"
#include <memory>
#include <iostream>

using namespace std;

//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#define PRINT_FLOATS(n,b,s) PrintFloats(n,b,s)
#define PRINT_COMPLEX(n,b,s) PrintComplex(n,b,s)
inline static void PrintFloats(const char* name, const float* buf, const int size) {
    for(int i = 0; i < size; i++) {
        cout << name << "[" << i << "] = " << buf[i] << endl;
    }
}

inline void PrintComplex(const char* name, const fftwf_complex* splits, const int size) {
    for(int i = 0; i < size; i++) {
        cout << name << "[" << i << "] = " << splits[i][0] << " + " << splits[i][1] << "i" << endl;
    }
}
#else
#define PRINT_FLOATS(n,b,s)
#define PRINT_COMPLEX(n,b,s)
#endif

AutoCorrelationW::AutoCorrelationW(int windowSize) :
IAutoCorrelation(windowSize),
_src(NULL),
_fft(NULL),
_powspec(NULL),
_ifft(NULL)
{
    _src = static_cast<float*>(fftwf_malloc(sizeof(float) * kFftSize));
    _fft = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * kFftSize));
    _powspec = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * kFftSize));
    _ifft = static_cast<float*>(fftwf_malloc(sizeof(float) * kFftSize));
    
    memset(_src, 0, sizeof(float) * kFftSize);
    memset(_fft, 0, sizeof(fftwf_complex) * kFftSize);
    memset(_powspec, 0, sizeof(fftwf_complex) * kFftSize);
    memset(_ifft, 0, sizeof(float) * kFftSize);
}

AutoCorrelationW::~AutoCorrelationW()
{
    fftwf_free(_src);
    fftwf_free(_fft);
    fftwf_free(_powspec);
    fftwf_free(_ifft);
}

void AutoCorrelationW::Compute(const float* x, float *corr)
{
    PRINT_FLOATS("x", x, kWinSize);
    
    // padding 0 half of src
    memcpy(_src, x, sizeof(float)*kWinSize);
    
    // fft
    fftwf_plan plan_fft = fftwf_plan_dft_r2c_1d(kFftSize, _src, _fft, FFTW_ESTIMATE);
    fftwf_execute(plan_fft);
    fftwf_destroy_plan(plan_fft);
    PRINT_COMPLEX("_fft", _fft, kFftSize);
    
    // power spectrum
    for (int i = 0; i < kFftSize; i++) {
        float powered = _fft[i][0] * _fft[i][0] + _fft[i][1] * _fft[i][1];
        _powspec[i][0] = powered / kFftSize;
        _powspec[i][1] = 0.0f;
    };
    PRINT_COMPLEX("_powspec", _powspec, kFftSize);
    
    // ifft
    fftwf_plan plan_ifft = fftwf_plan_dft_c2r_1d(kFftSize, _powspec, _ifft, FFTW_ESTIMATE);
    fftwf_execute(plan_ifft);
    fftwf_destroy_plan(plan_ifft);
    
    memcpy(corr, _ifft, sizeof(float)*kWinSize);
    PRINT_FLOATS("corr", corr, kWinSize);
}

