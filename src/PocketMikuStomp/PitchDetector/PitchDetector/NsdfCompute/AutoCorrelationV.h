//
//  AutoCorrelationV.h
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/08.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PitchDetector__AutoCorrelationV__
#define __PitchDetector__AutoCorrelationV__

#include "IAutoCorrelation.h"
#include <Accelerate/Accelerate.h>
#include <fftw3.h>// remove later

class AutoCorrelationV : public IAutoCorrelation
{
public:
    AutoCorrelationV(int windowSize);
    virtual ~AutoCorrelationV();
    void Compute(const float* x, float *corr);
private:
    float* _src;
    fftwf_complex* _fft;
    fftwf_complex* _powspec;
    float* _ifft;
	float* _temp;
	
    const unsigned int _log2n;
    DSPSplitComplex _fftv;
    DSPSplitComplex _ifftv;
    DSPSplitComplex _srcv;
    DSPSplitComplex _powspv;
    DSPSplitComplex _tempv;
};

#endif /* defined(__PitchDetector__AutoCorrelationV__) */
