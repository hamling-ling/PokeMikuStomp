#include "NsdfWalker.h"
#include <cassert>
#include "NsdfPeakDetectMachine.h"

namespace osakanaengine {

	NsdfWalker::NsdfWalker(uint32_t maxDataNum)
		: _maxDataNum(maxDataNum)
	{
		_sm = std::make_shared<NsdfPeakDetectMachine>(_maxDataNum);
		_sm->SetBellStartedHandler(boost::bind(&NsdfWalker::BellStarted, this, _1));
		_sm->SetBellFinishHandler(boost::bind(&NsdfWalker::BellFinished, this, _1));
	}

	NsdfWalker::~NsdfWalker()
	{
	}

	void NsdfWalker::Input(float x)
	{
		_sm->Input(x);
	}

	void NsdfWalker::GetKeyMaximums(std::vector<NsdfPoint>& out)
	{
		_sm->GetKeyMaximums(out);
	}

	void NsdfWalker::BellStarted(NsdfPeakDetectMachine* pdm)
	{
#if DEBUG_OUT
		std::cout << "bell started" << std::endl;
#endif
	}

	void NsdfWalker::BellFinished(NsdfPeakDetectMachine* pdm)
	{
#if DEBUG_OUT
		std::cout << "bell finished" << std::endl;
#endif
	}
}
