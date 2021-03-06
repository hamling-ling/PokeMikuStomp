//
//  PitchDetectorNullImpl.h
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#ifndef __PitchDetector__PitchDetectorNullImpl__
#define __PitchDetector__PitchDetectorNullImpl__

#include "PitchDetectorImpl.h"

class PitchDetectorNullImpl : public PitchDetectorImpl
{
public:
    PitchDetectorNullImpl(int samplingRate, int samplingSize);
    ~PitchDetectorNullImpl();
    virtual bool Initialize();
    
protected:
    virtual bool ComputeNsdf(const float* x);
};

#endif /* defined(__PitchDetector__PitchDetectorNullImpl__) */
