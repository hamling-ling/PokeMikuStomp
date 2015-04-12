//
//  PitchDetectorVDspImpl.cpp
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "PitchDetectorVDspImpl.h"
#include "AutoCorrelationV.h"
#include <iostream> // for debug

using namespace std;

PitchDetectorVDspImpl::PitchDetectorVDspImpl(int samplingRate, int samplingSize)
:
PitchDetectorImpl(samplingRate, samplingSize),
_2r(NULL)
{
}

PitchDetectorVDspImpl::~PitchDetectorVDspImpl()
{
    delete[] _2r;
}

bool PitchDetectorVDspImpl::PitchDetectorVDspImpl::Initialize()
{
    if(!PitchDetectorImpl::Initialize()) {
        return false;
    }
    
    std::shared_ptr<AutoCorrelationV> ptr = std::make_shared<AutoCorrelationV>(_samplingSize);
    _corr = std::dynamic_pointer_cast<IAutoCorrelation>(ptr);
    
    _2r = new float[_samplingSize];
    if(!_2r) {
        return false;
    }
    
    memset(_2r, 0, sizeof(float)*_samplingSize);
    
    return true;
}

bool PitchDetectorVDspImpl::ComputeNsdf(const float* x)
{
    const int N = _samplingSize;
    
    // compute square
    vDSP_vsq(x, 1, _x2, 1, _samplingSize);
    
    // auto correlation
    _corr->Compute(x, _r);
    
    // m(t)
    float sum_x2j = 0;
    float sum_x2jt = 0;
    for (int t = 0; t < N; t++) {
        vDSP_sve(_x2, 1, &sum_x2j, N-t);
        vDSP_sve(&(_x2[t]), 1, &sum_x2jt, N-t);
        _m[t] = sum_x2j + sum_x2jt;
    }
    
    // nsdf
    const float two = 2.0f;
    vDSP_vsmul(_r, 1, &two, _2r, 1, N);
    vDSP_vdiv(_m, 1, _2r, 1, _nsdf, 1, N);
    
    return true;
}
