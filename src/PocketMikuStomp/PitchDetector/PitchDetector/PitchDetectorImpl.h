//
//  PitchDetectorImpl.h
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PitchDetector__PitchDetectorImpl__
#define __PitchDetector__PitchDetectorImpl__

#include "PitchDetectorDefs.h"
#include <memory>

class IAutoCorrelation;

class PitchDetectorImpl
{
public:
    PitchDetectorImpl(int samplingRate, int samplingSize);
    virtual ~PitchDetectorImpl();
    virtual bool Initialize();
    virtual bool Detect(const float* x);
    virtual void GetPiatch(PitchInfo& pitch);
    int SamplingRate();
    int SamplingSize();
    
protected:
    const int _samplingRate;
    const int _samplingSize;
    std::shared_ptr<IAutoCorrelation> _corr;
    float* _r;
    float* _m;
    float* _x2;
    float* _nsdf;
	PitchInfo _pitch;
    
    virtual bool ComputeNsdf(const float* x) = 0;
    virtual int AnalyzeNsdf();
};

#endif /* defined(__PitchDetector__PitchDetectorImpl__) */
