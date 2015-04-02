#pragma once

#include <boost/bind/bind.hpp>
#include <boost/function.hpp>

#include "PeakDetectMsm.h"

namespace osakanaengine {
	class NsdfPeakDetectMachine;
	typedef boost::function<void(NsdfPeakDetectMachine*)> bellStartedCallback_t;
	typedef boost::function<void(NsdfPeakDetectMachine*)> bellFinishedCallback_t;

	/**
	 *	Wrapping meta state machine
	 */
	class NsdfPeakDetectMachine
	{
	public:
		NsdfPeakDetectMachine(uint32_t maxDataNum);
		virtual ~NsdfPeakDetectMachine();
		void SetBellStartedHandler(bellStartedCallback_t callback);
		void SetBellFinishHandler(bellFinishedCallback_t callback);
		void Input(float x);
		bool IsFinished();
		bool IsWalkingOnBell();
		void GetKeyMaximums(std::vector<NsdfPoint>& out);
	private:
		bellStartedCallback_t _bellStartedCallback;
		bellFinishedCallback_t _bellFinishedCallback;
		Sm1 _sm;
		void BellStarted();
		void BellFinished();
	};

}
