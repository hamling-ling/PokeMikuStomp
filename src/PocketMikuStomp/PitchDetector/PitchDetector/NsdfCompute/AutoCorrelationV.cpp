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

AutoCorrelationV::AutoCorrelationV(int windowSize) :
IAutoCorrelation(windowSize),
kLog2n(log2(windowSize)),
kOneOv1024(1.0f/1024.0f)
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
    // padding 0 half of src
    memcpy(_srcv.realp, x, sizeof(float)*kWinSize);
	
	// fft
    vDSP_fft_zop(_setup, &_srcv, 1, &_fftv, 1, kLog2n+1, FFT_FORWARD);
	
	// power spectrum
    vDSP_zvmags(&_fftv, 1, _tempv.realp, 1, kFftSize);
	vDSP_vsmul(_tempv.realp, 1, &kOneOv1024, _powspv.realp, 1, kFftSize);
	
	// inverse fft of powerspectrum
    vDSP_fft_zop(_setup, &_powspv, 1, &_ifftv, 1, kLog2n+1, FFT_INVERSE);
	
	memcpy(corr, _ifftv.realp, kWinSize);
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
