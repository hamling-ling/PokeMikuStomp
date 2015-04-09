//
//  PitchDetectorFftwImpl.h
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PitchDetector__PitchDetectorFftwImpl__
#define __PitchDetector__PitchDetectorFftwImpl__

#include "PitchDetectorImpl.h"

class PitchDetectorFftwImpl : public PitchDetectorImpl
{
public:
    PitchDetectorFftwImpl(int samplingRate, int samplingSize);
    virtual ~PitchDetectorFftwImpl();
    virtual bool Initialize();
    
private:
    virtual bool ComputeNsdf(const float* x);
};

#endif /* defined(__PitchDetector__PitchDetectorFftwImpl__) */
