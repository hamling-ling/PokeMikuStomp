//
//  AutoCorrelationV.cpp
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/08.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "AutoCorrelationV.h"
#include <fftw3.h>
#include <memory>
#include <iostream>

using namespace std;

AutoCorrelationV::AutoCorrelationV(int windowSize) :
IAutoCorrelation(windowSize),
_src(NULL),
_fft(NULL),
_powspec(NULL),
_ifft(NULL),
_log2n(log2(windowSize))
{
    _src = static_cast<float*>(fftwf_malloc(sizeof(float) * kFftSize));
    _fft = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * kFftSize));
    _powspec = static_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * kFftSize));
    _ifft = static_cast<float*>(fftwf_malloc(sizeof(float) * kFftSize));
    
    _srcv.realp = new float[kFftSize];
    _srcv.imagp = new float[kFftSize];
    _fftv.realp = new float[kFftSize];
    _fftv.imagp = new float[kFftSize];
    _ifftv.realp = new float[kFftSize];
    _ifftv.imagp = new float[kFftSize];
    _powspv.realp = new float[kFftSize];
    _powspv.imagp = new float[kFftSize];
    _tempv.realp = new float[kFftSize];
    _tempv.imagp = new float[kFftSize];
    
    memset(_srcv.realp, 0, sizeof(float)*kFftSize);
    memset(_srcv.imagp, 0, sizeof(float)*kFftSize);
    memset(_fftv.realp, 0, sizeof(float)*kFftSize);
    memset(_fftv.imagp, 0, sizeof(float)*kFftSize);
    memset(_ifftv.realp, 0, sizeof(float)*kFftSize);
    memset(_ifftv.imagp, 0, sizeof(float)*kFftSize);
    memset(_powspv.realp, 0, sizeof(float)*kFftSize);
    memset(_powspv.imagp, 0, sizeof(float)*kFftSize);
    memset(_tempv.realp, 0, sizeof(float)*kFftSize);
    memset(_tempv.imagp, 0, sizeof(float)*kFftSize);
}

AutoCorrelationV::~AutoCorrelationV()
{
    fftwf_free(_src);
    fftwf_free(_fft);
    fftwf_free(_powspec);
    fftwf_free(_ifft);
    
    delete[] (_srcv.realp);
    delete[] (_srcv.imagp);
    delete[] (_fftv.realp);
    delete[] (_fftv.imagp);
    delete[] (_ifftv.realp);
    delete[] (_ifftv.imagp);
    delete[] (_powspv.realp);
    delete[] (_powspv.imagp);
    delete[] (_tempv.realp);
    delete[] (_tempv.imagp);
}

void AutoCorrelationV::Compute(const float* x, float *corr)
{
    // debug
    for(int i = 0; i < 5; i++) {
        //cout << "x[" << i << "] = " << x[i] << endl;
    }
    
    memset(_src, 0, sizeof(float) * kFftSize);
    memset(_fft, 0, sizeof(fftwf_complex) * kFftSize);
    memset(_powspec, 0, sizeof(fftwf_complex) * kFftSize);
    
    // padding 0 half of src
    memcpy(_src, x, sizeof(float)*kWinSize);

    // fft
    fftwf_plan plan_fft = fftwf_plan_dft_r2c_1d(kFftSize, _src, _fft, FFTW_ESTIMATE);
    fftwf_execute(plan_fft);
    fftwf_destroy_plan(plan_fft);
    
    // debug
    for(int i = 0; i < 5; i++) {
        //cout << "_fft[" << i << "] = " << _fft[i][0] << " + " << _fft[i][1] << "i" << endl;
    }
    
    // power spectrum
    for (int i = 0; i < kFftSize; i++) {
        float powered = _fft[i][0] * _fft[i][0] + _fft[i][1] * _fft[i][1];
        _powspec[i][0] = powered / kFftSize;
        _powspec[i][1] = 0.0f;
    };
    
    // debug
    for(int i = 0; i < 5; i++) {
        cout << "_powspec[" << i << "] = " << _powspec[i][0]  << endl;
    }
    
    // ifft
    fftwf_plan plan_ifft = fftwf_plan_dft_c2r_1d(kFftSize, _powspec, _ifft, FFTW_ESTIMATE);
    fftwf_execute(plan_ifft);
    fftwf_destroy_plan(plan_ifft);
    
    // debug
    for(int i = 0; i < 5; i++) {
        cout << "ifft[" << i << "] = " << _ifft[i] << endl;
    }
    
    memcpy(corr, _ifft, sizeof(float)*kWinSize);
    
    // padding 0 half of src
    memcpy(_srcv.realp, x, sizeof(float)*kWinSize);
    
    // debug
    for(int i = 0; i < 5; i++) {
        //cout << "sp[" << i << "] = " << _srcv.realp[i]
       // << " + " << _srcv.imagp[i] << "i" << endl;
    }
    
    // setup
    FFTSetup setup = vDSP_create_fftsetup(_log2n+1, FFT_RADIX2);
    vDSP_fft_zop(setup, &_srcv, 1, &_fftv, 1, _log2n+1, FFT_FORWARD);
    
    // debug
    for(int i = 0; i < 5; i++) {
        //cout << "fft[" << i << "] = " << _fftv.realp[i]
        //    << " + " << _fftv.imagp[i] << "i" << endl;
    }
    
    //vDSP_vsq(_fftv.realp, 1, _tempv.realp, 1, kFftSize);
    //vDSP_vsq(_fftv.imagp, 1, _tempv.imagp, 1, kFftSize);
    //vDSP_vadd(_tempv.realp, 1, _tempv.imagp, 1, _powspv.realp, 1, kFftSize);
    vDSP_zvmags(&_fftv, 1, _powspv.realp, 1, kFftSize);
    
    // debug
    for(int i = 0; i < 5; i++) {
        cout << "pow[" << i << "] = " << _powspv.realp[i]
            << " + " << _powspv.imagp[i] << "i" << endl;
    }
    
    vDSP_fft_zop(setup, &_powspv, 1, &_ifftv, 1, _log2n+1, FFT_INVERSE);
    vDSP_zvabs(&_powspv, 1, corr, 1, kWinSize);
    float freq = 1.0/kFftSize;
    vDSP_vsmul(corr, 1, &freq, corr, 1, kWinSize);

    for(int i = 0; i < 5; i++) {
        //cout << "ifftv[" << i << "] = " << _ifftv.realp[i]
        //<< " + " << _ifftv.imagp[i] << "i" << endl;
        cout << "|ifft|[" << i << "] = " << corr[i] << endl;
    }
    
    vDSP_destroy_fftsetup(setup);
}

