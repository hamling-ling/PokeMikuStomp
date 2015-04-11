//
//  PitchDetectorImpl.cpp
//  PitchDetector
//
//  Created by Nobuhiro Kuroiwa on 2015/04/09.
//  Copyright (c) 2015年 Nobuhiro Kuroiwa. All rights reserved.
//

#include "PitchDetectorImpl.h"
#include "NsdfWalker.h"

using namespace std;
using namespace osakanaengine;

PitchDetectorImpl::PitchDetectorImpl(int samplingRate, int samplingSize)
:
_samplingRate(samplingRate),
_samplingSize(samplingSize),
_r(NULL), _m(NULL), _x2(NULL), _nsdf(NULL)
{
}

PitchDetectorImpl::~PitchDetectorImpl()
{
    delete[] (_r);
    delete[] (_m);
    delete[] (_x2);
    delete[] (_nsdf);
}

bool PitchDetectorImpl::Initialize()
{
    if (_corr) {
        return true;
    }
    
    _r = new float[_samplingSize];
    if (_r == NULL) {
        return false;
    }
    
    _m = new float[_samplingSize];
    if (_m == NULL) {
        return false;
    }
    
    _x2 = new float[_samplingSize];
    if (_x2 == NULL) {
        return false;
    }
    
    _nsdf = new float[_samplingSize];
    if (_nsdf == NULL) {
        return false;
    }
    
    return true;
}

bool PitchDetectorImpl::Detect(const float* x)
{
    if (!_corr) {
        return false;
    }
    
    if (!ComputeNsdf(x)) {
        return false;
    }
    
    if (AnalyzeNsdf() == -1) {
        return false;
    }
    
    return true;
}

void PitchDetectorImpl::GetPiatch(PitchInfo& pitch)
{
    pitch = _pitch;
}

int PitchDetectorImpl::SamplingRate()
{
    return _samplingRate;
}

int PitchDetectorImpl::SamplingSize()
{
    return _samplingSize;
}

int PitchDetectorImpl::AnalyzeNsdf()
{
    NsdfWalker walker(_samplingSize);
    
    for (int t = 0; t < _samplingSize; t++) {
        walker.Input(_nsdf[t]);
    }
    
    vector<NsdfPoint> keyMaxs;
    walker.GetKeyMaximums(keyMaxs);
    if (keyMaxs.empty()) {
        _pitch.error = true;
        return -1;
    }
    
    if (keyMaxs[0].index == 0.0) {
        _pitch.error = true;
        return -1;
    }
    
    float freq = _samplingRate/static_cast<float>(keyMaxs[0].index);
    
    float dnote = log10(freq / 27.5f) / kNoteConst;
    int inote = static_cast<int>(0.5f + dnote);
    if (inote < 0) {
        _pitch.error = true;
        return -1;
    }
    
    _pitch.note = static_cast<Note>(inote);
    _pitch.noteStr = kNoteStrings[inote % 12];
    _pitch.octave = inote / 12;
    _pitch.freq = freq;
    _pitch.midi = inote+21;
    _pitch.error = false;
    
    return inote;
}
