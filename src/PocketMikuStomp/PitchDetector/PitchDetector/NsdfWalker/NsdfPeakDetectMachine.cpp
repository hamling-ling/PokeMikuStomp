#include "NsdfPeakDetectMachine.h"

namespace osakanaengine {

	NsdfPeakDetectMachine::NsdfPeakDetectMachine(uint32_t maxDataNum)
	{
		_sm._bellStartCallback = boost::bind(&NsdfPeakDetectMachine::BellStarted, this);
		_sm._bellFinishCallback = boost::bind(&NsdfPeakDetectMachine::BellFinished, this);
		_sm.Initialize(maxDataNum);
		_sm.start();
	}

	NsdfPeakDetectMachine::~NsdfPeakDetectMachine()
	{
	}

	void NsdfPeakDetectMachine::SetBellStartedHandler(bellStartedCallback_t callback)
	{
		_bellStartedCallback = callback;
	}

	void NsdfPeakDetectMachine::SetBellFinishHandler(bellFinishedCallback_t callback)
	{
		_bellFinishedCallback = callback;
	}

	void NsdfPeakDetectMachine::Input(float x)
	{
#if DEBUG_OUT
		std::cout << "x[" << _sm._context.currentIndex << "]=" << x << std::endl;
#endif
		_sm.Input(x, _sm);
	}

	bool NsdfPeakDetectMachine::IsFinished()
	{
		int currentState = _sm.current_state()[0];
		return (Sm1_::kEndStateId == currentState);
	}

	void NsdfPeakDetectMachine::GetKeyMaximums(std::vector<NsdfPoint>& out)
	{
		_sm.GetKeyMaximums(out, _sm);
	}

	bool NsdfPeakDetectMachine::IsWalkingOnBell()
	{
		int currentState = _sm.current_state()[0];
		return (Sm1_::kWalkingOnBellStateId == currentState);
	}

	void NsdfPeakDetectMachine::BellStarted()
	{
		if (_bellStartedCallback) {
			_bellStartedCallback(this);
		}
	}

	void NsdfPeakDetectMachine::BellFinished()
	{
		if (_bellFinishedCallback) {
			_bellFinishedCallback(this);
		}
	}
}
