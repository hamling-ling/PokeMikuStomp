#include "PitchDetector.h"

#include "PitchDetectorNullImpl.h"

#define USE_VDSP
#ifdef USE_VDSP
#include "PitchDetectorVDspImpl.h"
#else
#include "PitchDetectorFftwImpl.h"
#endif

using namespace std;

PitchDetector::PitchDetector(int samplingRate, int samplingSize)
{
    std::shared_ptr<PitchDetectorNullImpl> ptr = std::make_shared<PitchDetectorNullImpl>(samplingRate, samplingSize);
    _impl = std::dynamic_pointer_cast<PitchDetectorImpl>(ptr);
}

PitchDetector::~PitchDetector()
{
}

bool PitchDetector::Initialize()
{
    int rate = _impl->SamplingRate();
    int size = _impl->SamplingSize();
#ifdef USE_VDSP
    std::shared_ptr<PitchDetectorVDspImpl> ptr = std::make_shared<PitchDetectorVDspImpl>(rate, size);
#else
    std::shared_ptr<PitchDetectorFftwImpl> ptr = std::make_shared<PitchDetectorFftwImpl>(rate, size);
#endif
    if(ptr->Initialize()) {
        _impl = std::dynamic_pointer_cast<PitchDetectorImpl>(ptr);
        return true;
    }

    return _impl->Initialize();
}

bool PitchDetector::Detect(const float* x)
{
    return _impl->Detect(x);
}

void PitchDetector::GetPiatch(PitchInfo& pitch)
{
    _impl->GetPiatch(pitch);
}
