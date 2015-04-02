#pragma once

#include <vector>
#include <cstdint>
#include <memory>

#include "NsdfWalkerDefs.h"

namespace osakanaengine {


	class NsdfPeakDetectMachine;

	/**
	 * finding maximums between positive zero-crossing and negative zero-crossing
	 * of Normalized square difference curve.
	 */
	class NsdfWalker
	{
	public:
		/**
		 * ctor
		 */
		NsdfWalker(uint32_t maxDataNum);

		/**
		 * dtor
		 */
		virtual ~NsdfWalker();

		/**
		 * handles input
		 */
		void Input(float x);

		/**
		 * returns key maximum. key maximums smaller than
		 * kThreshold * maximum are filtered out.
		 */
		void GetKeyMaximums(std::vector<NsdfPoint>& out);

	private:
		// max data number
		uint32_t _maxDataNum;
		// state machine
		std::shared_ptr<NsdfPeakDetectMachine> _sm;
		// call back handler from state machine event
		void BellStarted(NsdfPeakDetectMachine* pdm);
		// call back handler from state machine event
		void BellFinished(NsdfPeakDetectMachine* pdm);
	};

}
