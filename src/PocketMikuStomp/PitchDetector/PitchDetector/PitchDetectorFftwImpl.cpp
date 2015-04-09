//
//  PitchDetectorFftwImpl.cpp
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "PitchDetectorFftwImpl.h"
#include "NsdfCompute/AutoCorrelationW.h"

bool PitchDetectorFftwImpl::Initialize()
{
    if(!PitchDetectorImpl::Initialize()) {
        return false;
    }
    
    std::shared_ptr<AutoCorrelationW> ptr = std::make_shared<AutoCorrelationW>(_samplingSize);
    _corr = std::dynamic_pointer_cast<IAutoCorrelation>(ptr);
    
    return true;
}

bool PitchDetectorFftwImpl::ComputeNsdf(const float* x)
{
    const int N = _samplingSize;
    
    // compute square
    for (int i = 0; i < N; i++) {
        _x2[i] = powf(x[i], 2.0);
    }
    
    // auto correlation
    _corr->Compute(x, _r);
    
    // m(t)
    for (int t = 0; t < N; t++) {
        float sum = 0;
        for (int j = 0; j < N - t; j++) {
            sum += _x2[j] + _x2[j+t];
        }
        _m[t] = sum;
    }
    
    // nsdf
    for (int t = 0; t < N; t++) {
        _nsdf[t] = 2.0f * _r[t] / _m[t];
        
        // debug
        //std::cout << "nsdf[" << t << "]=" << _nsdf[t] << endl;
    }
    
    return true;
}
