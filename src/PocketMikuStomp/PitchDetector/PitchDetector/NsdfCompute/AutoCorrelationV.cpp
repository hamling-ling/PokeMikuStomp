//
//  AutoCorrelationV.cpp
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/08.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "AutoCorrelationV.h"
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

inline void PrintComplex(const char* name, const DSPSplitComplex* splits, const int size) {
    for(int i = 0; i < size; i++) {
        cout << name << "[" << i << "] = " << splits->realp[i] << " + " << splits->imagp[i] << "i" << endl;
    }
}
#else
#define PRINT_FLOATS(n,b,s)
#define PRINT_COMPLEX(n,b,s)
#endif

AutoCorrelationV::AutoCorrelationV(int windowSize) :
IAutoCorrelation(windowSize),
kLog2n(log2(windowSize)),
kFreq(1.0f/kFftSize)
{
	AllocateComplexBuf(_srcv, kFftSize);
	AllocateComplexBuf(_fftv, kFftSize);
	AllocateComplexBuf(_ifftv, kFftSize);
	AllocateComplexBuf(_powspv, kFftSize);
	AllocateComplexBuf(_tempv, kFftSize);
	_setup = vDSP_create_fftsetup(kLog2n+1, FFT_RADIX2);
}

AutoCorrelationV::~AutoCorrelationV()
{
	DestroyComplexBuf(_srcv);
	DestroyComplexBuf(_fftv);
	DestroyComplexBuf(_ifftv);
	DestroyComplexBuf(_powspv);
	DestroyComplexBuf(_tempv);
    vDSP_destroy_fftsetup(_setup);
}

void AutoCorrelationV::Compute(const float* x, float *corr)
{
    PRINT_FLOATS("X", x, kWinSize);
    
    // padding 0 half of src
    memcpy(_srcv.realp, x, sizeof(float)*kWinSize);
	
	// fft
    vDSP_fft_zop(_setup, &_srcv, 1, &_fftv, 1, kLog2n+1, FFT_FORWARD);
    PRINT_COMPLEX("_fftv", &_fftv, kFftSize);
    
	// power spectrum
    vDSP_zvmags(&_fftv, 1, _tempv.realp, 1, kFftSize);
	vDSP_vsmul(_tempv.realp, 1, &kFreq, _powspv.realp, 1, kFftSize);
    PRINT_COMPLEX("_powspv", &_powspv, kFftSize);
    
	// inverse fft of powerspectrum
    vDSP_fft_zop(_setup, &_powspv, 1, &_ifftv, 1, kLog2n+1, FFT_INVERSE);
	
	memcpy(corr, _ifftv.realp, sizeof(float) * kWinSize);
    
    PRINT_FLOATS("corr", corr, kWinSize);
}

bool AutoCorrelationV::AllocateComplexBuf(DSPSplitComplex& c, int len)
{
	c.realp = new float[len];
	if(!c.realp) {
		return false;
	}
	c.imagp = new float[len];
	if(!c.imagp) {
		return false;
	}
	
	memset(c.realp, 0, sizeof(float)*len);
	memset(c.imagp, 0, sizeof(float)*len);
	
	return true;
}

void AutoCorrelationV::DestroyComplexBuf(DSPSplitComplex& c)
{
	delete[] c.realp;
	delete[] c.imagp;
	c.realp = NULL;
	c.imagp = NULL;
}
