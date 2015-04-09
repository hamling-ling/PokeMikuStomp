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
    const unsigned int kLog2n;
	const float kOneOv1024;
    DSPSplitComplex _fftv;
    DSPSplitComplex _ifftv;
    DSPSplitComplex _srcv;
    DSPSplitComplex _powspv;
    DSPSplitComplex _tempv;
	FFTSetup _setup;
	
	bool AllocateComplexBuf(DSPSplitComplex& c, int len);
	void DestroyComplexBuf(DSPSplitComplex& c);
};

#endif /* defined(__PitchDetector__AutoCorrelationV__) */
