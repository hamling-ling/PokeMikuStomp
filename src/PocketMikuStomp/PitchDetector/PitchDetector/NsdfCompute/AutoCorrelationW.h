//
//  AutoCorrelationW.h
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/08.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PitchDetector__AutoCorrelationW__
#define __PitchDetector__AutoCorrelationW__

#include <fftw3.h>
#include "IAutoCorrelation.h"

class AutoCorrelationW : public IAutoCorrelation
{
public:
    AutoCorrelationW(int windowSize);
    virtual ~AutoCorrelationW();
    void Compute(const float* x, float *corr);
private:
    float* _src;
    fftwf_complex* _fft;
    fftwf_complex* _powspec;
    float* _ifft;
};


#endif /* defined(__PitchDetector__AutoCorrelationW__) */
