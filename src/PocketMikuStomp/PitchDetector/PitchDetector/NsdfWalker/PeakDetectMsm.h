#pragma once

#include <iostream>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include "NsdfWalkerDefs.h"

namespace osakanaengine {
	namespace msm = boost::msm;
	namespace msmf = boost::msm::front;
	namespace mpl = boost::mpl;

	// ----- Events
	struct PosCross  {};
	struct NegCross  {};
	struct NomlData  {};
	struct EndOfData {};

	// ----- Context
	struct MachineContext
	{
		uint32_t maxDataNum;
		uint32_t currentIndex;
		NsdfPoint lastInput;
		// collection of key maximum for each bell
		std::vector<NsdfPoint> keyMaxs;
		// max of all bell
		NsdfPoint globalKeyMax;
		// max of current bell
		NsdfPoint localKeyMax;
		// ctor
		MachineContext() {
			maxDataNum = 0;
			currentIndex = 0;
			memset(&lastInput, 0, sizeof(lastInput));
			memset(&globalKeyMax, 0, sizeof(globalKeyMax));
			memset(&localKeyMax, 0, sizeof(localKeyMax));
		}
	};

	// ----- State machine
	struct Sm1_ : msmf::state_machine_def < Sm1_ >
	{
		// member variables
		boost::function<void(void)> _bellStartCallback;
		boost::function<void(void)> _bellFinishCallback;
		MachineContext _context;

		// States
		struct SmState : msmf::state < >
		{
			virtual void Input(float x, msm::back::state_machine<Sm1_>& sm) {
				MachineContext& context = sm._context;

				context.lastInput.value = x;
				context.lastInput.index = context.currentIndex;

				context.currentIndex++;

				if (context.maxDataNum <= context.currentIndex) {
					sm.process_event(EndOfData());
				}
			}
		};

		struct SearchingBell : SmState
		{
			virtual void Input(float x, msm::back::state_machine<Sm1_>& sm) {
				MachineContext& context = sm._context;

				if (context.lastInput.value < 0 && 0 <= x) {
					sm.process_event(PosCross());
				}
				else {
					sm.process_event(NomlData());
				}

				SmState::Input(x, sm);
			}
		};

		struct WalkingOnBell : SmState
		{
			virtual void Input(float x, msm::back::state_machine<Sm1_>& sm) {
				MachineContext& context = sm._context;

				if (0 <= context.lastInput.value && x < 0) {
					sm.process_event(NegCross());
				}
				else {
					sm.process_event(NomlData());
				}

				// update local key max
					if (context.localKeyMax.value < x) {
					context.localKeyMax.value = x;
					context.localKeyMax.index = context.currentIndex;
				}

				// update global key max
					if (context.globalKeyMax.value < x) {
					context.globalKeyMax.value = x;
					context.globalKeyMax.index = context.currentIndex;
				}

				SmState::Input(x, sm);
			}
		};

		struct End : SmState
		{
			virtual void Input(float x, msm::back::state_machine<Sm1_>& sm) {
				// do nothing
			}
		};

		// Member Functions

		/**
		* ctor
		*/
		Sm1_() {}

		/**
		* Initialize State Machine
		*/
		void Initialize(uint32_t maxDataNum) {
			assert(maxDataNum != 0);
			_context.maxDataNum = maxDataNum;
			_context.keyMaxs.reserve(kReservedBellNumber);
		}

		/**
		* handles input
		*/
		void Input(float x, msm::back::state_machine<Sm1_>& sm) {
			int stateId = sm.current_state()[0];
			SmState* state = static_cast<Sm1_::SmState*>(sm.get_state_by_id(stateId));

			state->Input(x, sm);
		}

		/**
		* returns key maximum. key maximums smaller than
		* kThreshold * maximum are filtered out.
		*/
		void GetKeyMaximums(std::vector<NsdfPoint>& out, msm::back::state_machine<Sm1_>& sm)
		{
			for (auto keyMax : sm._context.keyMaxs) {
				if (sm._context.globalKeyMax.value * kThreshold <= keyMax.value) {
					out.push_back(keyMax);
				}
			}
		}

		// Set initial state
		typedef SearchingBell initial_state;

		// Actions
		struct BellStarted {

			template <class Event, class Fsm, class SourceState, class TargetState>
			void operator()(Event const&, Fsm& fsm, SourceState&, TargetState&) const
			{
				Sm1_* sm1 = &fsm;
				if (sm1->_bellStartCallback) {
					sm1->_bellStartCallback();
				}
			}
		};

		struct BellFinished {
			template <class Event, class Fsm, class SourceState, class TargetState>
			void operator()(Event const&, Fsm& fsm, SourceState&, TargetState&) const
			{
				Sm1_* sm1 = &fsm;
				MachineContext& context = sm1->_context;

				context.keyMaxs.push_back(context.localKeyMax);

#ifdef DEBUG_OUT
				std::cout << "bell finished maximum(value,index)=("
					<< context.localKeyMax.value << ", " << context.localKeyMax.index << ")" << std::endl;
#endif

				// reset local key maximum
				context.localKeyMax.index = 0;
				context.localKeyMax.value = 0;

				if (sm1->_bellFinishCallback) {
					sm1->_bellFinishCallback();
				}
			}
		};

		// Transition table
		struct transition_table : mpl::vector<
			//          Start          Event      Next           Action        Guard
			msmf::Row < SearchingBell, PosCross,  WalkingOnBell, BellStarted,  msmf::none >,
			msmf::Row < SearchingBell, NegCross,  SearchingBell, msmf::none,   msmf::none >,
			msmf::Row < SearchingBell, NomlData,  SearchingBell, msmf::none,   msmf::none >,
			msmf::Row < SearchingBell, EndOfData, End,           msmf::none,   msmf::none >,

			msmf::Row < WalkingOnBell, PosCross,  WalkingOnBell, msmf::none,   msmf::none >,
			msmf::Row < WalkingOnBell, NegCross,  SearchingBell, BellFinished, msmf::none >,
			msmf::Row < WalkingOnBell, NomlData,  WalkingOnBell, msmf::none,   msmf::none >,
			msmf::Row < WalkingOnBell, EndOfData, End,           BellFinished, msmf::none >,

			msmf::Row < End,           PosCross,  End,           msmf::none,   msmf::none >,
			msmf::Row < End,           NegCross,  End,           msmf::none,   msmf::none >,
			msmf::Row < End,           NomlData,  End,           msmf::none,   msmf::none >,
			msmf::Row < End,           EndOfData, End,           msmf::none,   msmf::none >
		> {};

		// ----- State IDs
		typedef enum _MetaStateId {
			kSearchingBellStateId = 0,
			kWalkingOnBellStateId = 1,
			kEndStateId = 2
		} MetaStateId;
	};

	// Pick a back-end
	typedef msm::back::state_machine<Sm1_> Sm1;
}
