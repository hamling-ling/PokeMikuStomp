//
//  PitchDetectorVDspImpl.h
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PitchDetector__PitchDetectorVDspImpl__
#define __PitchDetector__PitchDetectorVDspImpl__

#include "PitchDetectorImpl.h"

class PitchDetectorVDspImpl : public PitchDetectorImpl
{
public:
    PitchDetectorVDspImpl(int samplingRate, int samplingSize);
    virtual ~PitchDetectorVDspImpl();
    virtual bool Initialize();
    
private:
    float* _2r;
    virtual bool ComputeNsdf(const float* x);
};

#endif /* defined(__PitchDetector__PitchDetectorVDspImpl__) */
