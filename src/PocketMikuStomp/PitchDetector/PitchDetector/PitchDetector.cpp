#include "PitchDetector.h"

#include <iostream>
#include <cmath>
#include "NsdfCompute/IAutoCorrelation.h"
#include "NsdfCompute/AutoCorrelationV.h"
#include "NsdfWalker/NsdfWalker.h"

#define USE_VDSP
#ifdef USE_VDSP
#include <Accelerate/Accelerate.h>
#endif

using namespace std;
using namespace osakanaengine;

static const float kNoteConst = log10(pow(2.0f, 1.0f / 12.0f));
const char* kNoteStrings[] {
	"A", "B♭", "B", "C",
	"C#", "D", "D#", "E",
	"F", "F#", "G", "G#",
};

PitchDetector::PitchDetector(int samplingRate, int samplingSize)
: 
_samplingRate(samplingRate),
_samplingSize(samplingSize),
_r(NULL), _m(NULL), _x(NULL), _x2(NULL), _nsdf(NULL)
{
}

PitchDetector::~PitchDetector()
{
	delete[] (_r);
	delete[] (_m);
	delete[] (_x);
	delete[] (_x2);
	delete[] (_nsdf);
}

bool PitchDetector::Initialize()
{
	if (_corr) {
		return true;
	}

    std::shared_ptr<AutoCorrelationV> ptr = std::make_shared<AutoCorrelationV>(_samplingSize);
    _corr = std::dynamic_pointer_cast<IAutoCorrelation>(ptr);
	_r = new float[_samplingSize];
	if (_r == NULL) {
		return false;
	}

	_m = new float[_samplingSize];
	if (_m == NULL) {
		return false;
	}

	_x = new float[_samplingSize];
	if (_x == NULL) {
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

bool PitchDetector::Detect(const float* x)
{
	if (!_corr) {
		return false;
	}

#ifdef USE_VDSP
    //vDSP_vmul(x, 1, x, 1, _x2, 1, _samplingSize);
    vDSP_vsq(x, 1, _x2, 1, _samplingSize);
#else
	for (int i = 0; i < _samplingSize; i++) {
		_x2[i] = powf(x[i], 2.0);
	}
#endif
    
	if (!ComputeNsdf(x, _x2)) {
		return false;
	}

	if (AnalyzeNsdf() == -1) {
		return false;
	}

	return true;
}

void PitchDetector::GetPiatch(PitchInfo& pitch)
{
	pitch = _pitch;
}

bool PitchDetector::ComputeNsdf(const float* x, const float* x2)
{
	const int N = _samplingSize;

	// auto correlation
	_corr->Compute(x, _r);

	// m(t)
	memset(_m, 0, sizeof(float)* N);
	for (int t = 0; t < N; t++) {
		for (int j = 0; j < N - t; j++) {
			_m[t] = _m[t] + x2[j] + x2[j + t];
		}
	}

	// nsdf
#ifdef USE_VDSP
    float two = 2.0f;
    vDSP_vsmul(_r, 1, &two, _nsdf, 1, N);
    vDSP_vdiv(_m, 1, _nsdf, 1, _nsdf, 1, N);
#else
	for (int t = 0; t < N; t++) {
		_nsdf[t] = 2.0f * _r[t] / _m[t];

		// debug
		//std::cout << "nsdf[" << t << "]=" << _nsdf[t] << endl;
	}
#endif
    
	return true;
}

int PitchDetector::AnalyzeNsdf()
{
	NsdfWalker walker(_samplingSize);

	for (int t = 0; t < _samplingSize; t++) {
		walker.Input(_nsdf[t]);
	}

	vector<NsdfPoint> keyMaxs;
	walker.GetKeyMaximums(keyMaxs);
	if (keyMaxs.empty()) {
		return -1;
	}

	if (keyMaxs[0].index == 0.0) {
		return -1;
	}

	float freq = _samplingRate/static_cast<float>(keyMaxs[0].index);

	float dnote = log10(freq / 27.5f) / kNoteConst;
	int inote = static_cast<int>(0.5f + dnote);
	if (inote < 0) {
		return -1;
	}

	_pitch.note = static_cast<Note>(inote);
	_pitch.noteStr = kNoteStrings[inote % 12];
	_pitch.octave = inote / 12;
	_pitch.freq = freq;
    _pitch.midi = inote+21;
    
	return inote;
}
