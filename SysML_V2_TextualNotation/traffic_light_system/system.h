/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

// Command line options: -d -l cppx -p sysml2text -o system tl.sysml 
// This file is generated from tl.sysml - do not edit manually
// Generated on: Mon Mar 16 19:57:59 CET 2026 / 7.1


#pragma once


// Standard C++ includes
#include <iostream>
#include <memory>
#include <variant> // C++17
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <set>
#include <cassert>

#include <framework.h>

namespace TrafficLight{
	enum class TLCEvent : std::int16_t {
	None = 0,  // sentinel for "no value"
	evError,
	evOperational,
	evTimeout1,
	evTimeout2,
	evTimeout3,
	evTimeout4,
	evTimeout5,
	evTimeout6
};
	enum class TMCEvent : std::int16_t {
	None = 0,  // sentinel for "no value"
	evTimeout0
};
	
class PortData {
public:
	TLCEvent msg;

    // Constructors
    PortData(TLCEvent msgVal)
		: msg(msgVal) {}

	PortData() : msg(TLCEvent::None){}	
};

struct ControlPortDataDef {
    PortData data;
};

using ControlPortData  = std::variant<ControlPortDataDef>;

	

	
	
	
class Test : public Part {

public:


	Test() {
	}



	


 	virtual void process() override {

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void) override{			
		
 	}






};


	
class TrafficManagementCenter : public Part {

public:


	TrafficManagementCenter() {
	}



	Port<ControlPortData> sendPort;


	//elements of this part	

	TMCEvent msg  = TMCEvent::None ;
	TimeoutManager::TimeoutHandle timeoutId0  = -1 ;



	// states of the part's state machine
	enum class States : std::uint16_t {
		Operational,
		PreOperational,
		tmcStateMachine,
		NUM_STATES  // number of states in the machine
	};



	// state machine related variables (auto generated)
	// Flag if state machine was already initialized
	int m_initialized=0;

	// State variables
	struct stateVarsT{
		States stateVar;
		States stateVartmcStateMachine;
	};

	stateVarsT stateVars;
	stateVarsT stateVarsCopy;


	

	std::unique_ptr<Test> test1 = std::make_unique<Test>();


    virtual void onTimeout(const TMCEvent & event) {assert(false);} ;

 	virtual void process() override {

		test1->process();

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void) override{			
		assert(test1);
		test1->init();
		
 	}


 	// State machine initialize function. Must be called once before event processing.
 	void initialize(void){
 		//call on entry code of default states
 		if(m_initialized==static_cast<int>(0U)){
 			m_initialized=static_cast<int>(1U);

 			//Create copy of statevar
 			stateVarsCopy = stateVars;

 			// Set state vars to default states

 			stateVarsCopy.stateVar = States::tmcStateMachine; /* set init state of top state */
 			stateVarsCopy.stateVartmcStateMachine = States::PreOperational; /* set init state of tmcStateMachine */



 			//Automatically generated from accept after transition
 			TMCEvent evTimeout0Message = TMCEvent::evTimeout0;
 			timeoutId0 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(2.0,"SI::second")), [this, evTimeout0Message](){onTimeout(evTimeout0Message);});
 			

 			// Copy state variables back
 			stateVars = stateVarsCopy;

 		}

 	}


 	// Debug support functions

 	/* State names */
 	const std::array<std::string, 3> stateNames = { \
 		"Operational","PreOperational","tmcStateMachine"};



 	/* Event names */
 	const std::array<std::string, 3> eventNames = { \
 		"None","evTimeout0","NO_MSG"};

 	const std::string getNameByState(const States state) const {
 		return (static_cast<const std::string>(stateNames[static_cast<std::uint16_t>(state)]));
 	}

 	const std::string getNameByEvent(const TMCEvent evt) const {
 		return (static_cast<const std::string>(eventNames[static_cast<std::int16_t>(evt)]));
 	}



 	// Reporting functions
 	bool isInOperational(void) const {return(((stateVars.stateVartmcStateMachine == States::Operational) && (stateVars.stateVar == States::tmcStateMachine)) ? (true) : (false));}


 	bool isInPreOperational(void) const {return(((stateVars.stateVartmcStateMachine == States::PreOperational) && (stateVars.stateVar == States::tmcStateMachine)) ? (true) : (false));}


 	bool isIntmcStateMachine(void) const {return(((stateVars.stateVar == States::tmcStateMachine)) ? (true) : (false));}



 	// Return innermost active states
 	const std::forward_list<States> getInnermostActiveStates(void) const {
 		std::forward_list<States> stateList;

 		if ( isInPreOperational() ) {stateList.push_front(States::PreOperational);}
 		if ( isInOperational() ) {stateList.push_front(States::Operational);}
 		return stateList;
 	}



 	// State machine handler function
 	void tmcStateMachine(void){
 		//Flag whether event was consumed
 		int  evConsumed=0;

 		//Check whether the machine is already initialized
 		if(m_initialized==static_cast<int>(0U)){
 			m_initialized=static_cast<int>(1U);
 			initialize();
 		}

 		//Create copy of statevar
 		stateVarsCopy = stateVars;


 		switch (stateVars.stateVar) {

 			case States::tmcStateMachine:

 				switch (stateVars.stateVartmcStateMachine) {

 					case States::PreOperational:
 						if(msg==TMCEvent::evTimeout0){
 							/* Transition from PreOperational to Operational */
 							evConsumed = static_cast<int>(1U);
 							/* OnExit code of state PreOperational */
 							tm.cancelTimer(timeoutId0);

 							/* Action code for transition  */
 							sendPort.send(ControlPortDataDef{{TLCEvent::evOperational}});


 							/* adjust state variables  */
 							stateVarsCopy.stateVartmcStateMachine = States::Operational;
 						}else{
 							/* Intentionally left blank */
 						} /*end of event selection */
 					break; /* end of case PreOperational  */

 					case States::Operational:
 					break; /* end of case Operational  */

 					default:
 						/* Intentionally left blank */
 					break;
 				} /* end switch tmcStateMachine */
 			break; /* end of case tmcStateMachine  */

 			default:
 				/* Intentionally left blank */
 			break;
 		} /* end switch stateVar_root */

 		// Copy state variables back
 		stateVars = stateVarsCopy;

 	} // end state machine handler


};


	
class TrafficLightController : public Part {

public:


	TrafficLightController() {
		recvPort.addHandler([this](const ControlPortData& e){
			std::visit([this](auto&& ev) { handle(ev); }, e);
		});

	}

	// called by process() to handle received event
	virtual void handle(const ControlPortDataDef& e) {assert(false);};



	InputPort<ControlPortData> recvPort;


	//elements of this part	

	int redtime  = 1 ;
	TLCEvent msg  = TLCEvent::None ;
	TimeoutManager::TimeoutHandle timeoutId1  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId2  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId3  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId4  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId5  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId6  = -1 ;



	// states of the part's state machine
	enum class States : std::uint16_t {
		Operational,
		OperationalGreen,
		OperationalRed,
		OperationalRedYellow,
		OperationalYellow,
		OutOfService,
		OutOfServiceYellowOff,
		OutOfServiceYellowOn,
		tlcStateMachine,
		NUM_STATES  // number of states in the machine
	};



	// state machine related variables (auto generated)
	// Flag if state machine was already initialized
	int m_initialized=0;

	// State variables
	struct stateVarsT{
		States stateVar;
		States stateVartlcStateMachine;
		States stateVarOutOfService;
		States stateVarOperational;
	};

	stateVarsT stateVars;
	stateVarsT stateVarsCopy;


	

	std::unique_ptr<Test> test1 = std::make_unique<Test>();


    virtual void onTimeout(const TLCEvent & event) {assert(false);} ;

 	virtual void process() override {
		recvPort.process();


		test1->process();

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void) override{			
		assert(test1);
		test1->init();
		
 	}


 	struct readEventDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: readEvent.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;



 			enum class readEventID {

 				assignmentNode0, 
 				ImplicitStartNode0,
 				__END__
 			};


 			readEventID id = readEventID::assignmentNode0;

 			struct readEventContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				readEventContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 			while(id !=readEventID::__END__ ){ 
 				switch(id) {
 					case readEventID::assignmentNode0:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = readEventID::__END__;
 						msg = recvPort.data.msg;
 						break;

 					default:
 						break;

 				} //end switch
 			} //end while
 		} // end run method
 		virtual ~readEventDef() = default;

 	};//readEvent


 	struct setRedLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: setRedLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct setRedLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				setRedLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~setRedLEDDef() = default;

 	};//setRedLED


 	struct resetRedLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: resetRedLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct resetRedLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				resetRedLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~resetRedLEDDef() = default;

 	};//resetRedLED


 	struct setYellowLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: setYellowLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct setYellowLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				setYellowLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~setYellowLEDDef() = default;

 	};//setYellowLED


 	struct resetYellowLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: resetYellowLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct resetYellowLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				resetYellowLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~resetYellowLEDDef() = default;

 	};//resetYellowLED


 	struct setGreenLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: setGreenLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct setGreenLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				setGreenLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~setGreenLEDDef() = default;

 	};//setGreenLED


 	struct resetGreenLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: resetGreenLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct resetGreenLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				resetGreenLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~resetGreenLEDDef() = default;

 	};//resetGreenLED


 	struct setRedAndYellowLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: setRedAndYellowLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct setRedAndYellowLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				setRedAndYellowLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~setRedAndYellowLEDDef() = default;

 	};//setRedAndYellowLED


 	struct resetRedAndYellowLEDDef {
 		// Top-level activity: parent is the part
 		TrafficLightController* part_ptr_ = nullptr;
 		TrafficLightController& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(TrafficLightController& parent_){
 			std::cout << "debug: resetRedAndYellowLED.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct resetRedAndYellowLEDContext{
 				//owning part
 				TrafficLightController& parent;
 				//indirect accessible features
 				int& redtime;
 				TLCEvent& msg;
 				TimeoutManager::TimeoutHandle& timeoutId1;
 				TimeoutManager::TimeoutHandle& timeoutId2;
 				TimeoutManager::TimeoutHandle& timeoutId3;
 				TimeoutManager::TimeoutHandle& timeoutId4;
 				TimeoutManager::TimeoutHandle& timeoutId5;
 				TimeoutManager::TimeoutHandle& timeoutId6;
 				Test& test1;
 				InputPort<ControlPortData>& recvPort;

 				resetRedAndYellowLEDContext(TrafficLightController& parent_ 
 					) : parent(parent_)
 					,
 					redtime(parent_.redtime),
 					msg(parent_.msg),
 					timeoutId1(parent_.timeoutId1),
 					timeoutId2(parent_.timeoutId2),
 					timeoutId3(parent_.timeoutId3),
 					timeoutId4(parent_.timeoutId4),
 					timeoutId5(parent_.timeoutId5),
 					timeoutId6(parent_.timeoutId6),
 					test1(*parent_.test1),
 					recvPort(parent_.recvPort){}
 			}ctx{parent_};

 			//make context available
 			auto& redtime = ctx.redtime;
 			auto& msg = ctx.msg;
 			auto& timeoutId1 = ctx.timeoutId1;
 			auto& timeoutId2 = ctx.timeoutId2;
 			auto& timeoutId3 = ctx.timeoutId3;
 			auto& timeoutId4 = ctx.timeoutId4;
 			auto& timeoutId5 = ctx.timeoutId5;
 			auto& timeoutId6 = ctx.timeoutId6;
 			auto& test1 = ctx.test1;
 			auto& recvPort = ctx.recvPort;

 		} // end run method
 		virtual ~resetRedAndYellowLEDDef() = default;

 	};//resetRedAndYellowLED



 	// State machine initialize function. Must be called once before event processing.
 	void initialize(void){
 		//call on entry code of default states
 		if(m_initialized==static_cast<int>(0U)){
 			m_initialized=static_cast<int>(1U);

 			//Create copy of statevar
 			stateVarsCopy = stateVars;

 			// Set state vars to default states

 			stateVarsCopy.stateVar = States::tlcStateMachine; /* set init state of top state */
 			stateVarsCopy.stateVartlcStateMachine = States::OutOfService; /* set init state of tlcStateMachine */
 			stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn; /* set init state of OutOfService */
 			stateVarsCopy.stateVarOperational = States::OperationalRed; /* set init state of Operational */



 			{
 				setYellowLEDDef setYellowLED;
 				setYellowLED(*this);
 			}
 			
 			//Automatically generated from accept after transition
 			TLCEvent evTimeout1Message = TLCEvent::evTimeout1;
 			timeoutId1 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout1Message](){onTimeout(evTimeout1Message);});
 			

 			// Copy state variables back
 			stateVars = stateVarsCopy;

 		}

 	}


 	// Debug support functions

 	/* State names */
 	const std::array<std::string, 9> stateNames = { \
 		"Operational","OperationalGreen","OperationalRed","OperationalRedYellow","OperationalYellow","OutOfService","OutOfServiceYellowOff","OutOfServiceYellowOn","tlcStateMachine"};



 	/* Event names */
 	const std::array<std::string, 10> eventNames = { \
 		"None","evError","evOperational","evTimeout1","evTimeout2","evTimeout3","evTimeout4","evTimeout5","evTimeout6","NO_MSG"};

 	const std::string getNameByState(const States state) const {
 		return (static_cast<const std::string>(stateNames[static_cast<std::uint16_t>(state)]));
 	}

 	const std::string getNameByEvent(const TLCEvent evt) const {
 		return (static_cast<const std::string>(eventNames[static_cast<std::int16_t>(evt)]));
 	}



 	// Reporting functions
 	bool isInOperational(void) const {return(((stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOperationalGreen(void) const {return(((stateVars.stateVarOperational == States::OperationalGreen) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOperationalRed(void) const {return(((stateVars.stateVarOperational == States::OperationalRed) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOperationalRedYellow(void) const {return(((stateVars.stateVarOperational == States::OperationalRedYellow) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOperationalYellow(void) const {return(((stateVars.stateVarOperational == States::OperationalYellow) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOutOfService(void) const {return(((stateVars.stateVartlcStateMachine == States::OutOfService) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOutOfServiceYellowOff(void) const {return(((stateVars.stateVarOutOfService == States::OutOfServiceYellowOff) && (stateVars.stateVartlcStateMachine == States::OutOfService) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isInOutOfServiceYellowOn(void) const {return(((stateVars.stateVarOutOfService == States::OutOfServiceYellowOn) && (stateVars.stateVartlcStateMachine == States::OutOfService) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


 	bool isIntlcStateMachine(void) const {return(((stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}



 	// Return innermost active states
 	const std::forward_list<States> getInnermostActiveStates(void) const {
 		std::forward_list<States> stateList;

 		if ( isInOutOfServiceYellowOn() ) {stateList.push_front(States::OutOfServiceYellowOn);}
 		if ( isInOutOfServiceYellowOff() ) {stateList.push_front(States::OutOfServiceYellowOff);}
 		if ( isInOperationalRed() ) {stateList.push_front(States::OperationalRed);}
 		if ( isInOperationalRedYellow() ) {stateList.push_front(States::OperationalRedYellow);}
 		if ( isInOperationalGreen() ) {stateList.push_front(States::OperationalGreen);}
 		if ( isInOperationalYellow() ) {stateList.push_front(States::OperationalYellow);}
 		return stateList;
 	}



 	// State machine handler function
 	void tlcStateMachine(void){
 		//Flag whether event was consumed
 		int  evConsumed=0;

 		//Check whether the machine is already initialized
 		if(m_initialized==static_cast<int>(0U)){
 			m_initialized=static_cast<int>(1U);
 			initialize();
 		}

 		//Create copy of statevar
 		stateVarsCopy = stateVars;


 		switch (stateVars.stateVar) {

 			case States::tlcStateMachine:
 				/* action code  */
 				{
 					readEventDef readEvent;
 					readEvent(*this);
 				}
 				



 				switch (stateVars.stateVartlcStateMachine) {

 					case States::OutOfService:

 						switch (stateVars.stateVarOutOfService) {

 							case States::OutOfServiceYellowOn:
 								if(msg==TLCEvent::evTimeout1){
 									/* Transition from OutOfServiceYellowOn to OutOfServiceYellowOff */
 									evConsumed = static_cast<int>(1U);
 									/* OnExit code of state OutOfServiceYellowOn */
 									tm.cancelTimer(timeoutId1);

 									/* OnEntry code of state OutOfServiceYellowOff */
 									{
 										resetYellowLEDDef resetYellowLED;
 										resetYellowLED(*this);
 									}
 									
 									//Automatically generated from accept after transition
 									TLCEvent evTimeout2Message = TLCEvent::evTimeout2;
 									timeoutId2 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout2Message](){onTimeout(evTimeout2Message);});

 									/* adjust state variables  */
 									stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOff;
 								}else{
 									/* Intentionally left blank */
 								} /*end of event selection */
 							break; /* end of case OutOfServiceYellowOn  */

 							case States::OutOfServiceYellowOff:
 								if(msg==TLCEvent::evTimeout2){
 									/* Transition from OutOfServiceYellowOff to OutOfServiceYellowOn */
 									evConsumed = static_cast<int>(1U);
 									/* OnExit code of state OutOfServiceYellowOff */
 									tm.cancelTimer(timeoutId2);

 									/* OnEntry code of state OutOfServiceYellowOn */
 									{
 										setYellowLEDDef setYellowLED;
 										setYellowLED(*this);
 									}
 									
 									//Automatically generated from accept after transition
 									TLCEvent evTimeout1Message = TLCEvent::evTimeout1;
 									timeoutId1 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout1Message](){onTimeout(evTimeout1Message);});

 									/* adjust state variables  */
 									stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn;
 								}else{
 									/* Intentionally left blank */
 								} /*end of event selection */
 							break; /* end of case OutOfServiceYellowOff  */

 							default:
 								/* Intentionally left blank */
 							break;
 						} /* end switch OutOfService */

 						/* Check if event was already processed  */
 						if(evConsumed==static_cast<int>(0)){

 							if(msg==TLCEvent::evOperational){
 								/* Transition from OutOfService to Operational */
 								evConsumed = static_cast<int>(1U);
 								
 								if(stateVars.stateVarOutOfService == States::OutOfServiceYellowOn){
 									tm.cancelTimer(timeoutId1);


 								}else if(stateVars.stateVarOutOfService == States::OutOfServiceYellowOff){
 									tm.cancelTimer(timeoutId2);

 								}else {
 									/* Intentionally left blank */
 								};


 								{
 									setRedLEDDef setRedLED;
 									setRedLED(*this);
 								}
 								
 								//Automatically generated from accept after transition
 								TLCEvent evTimeout3Message = TLCEvent::evTimeout3;
 								timeoutId3 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(redtime,"SI::second")), [this, evTimeout3Message](){onTimeout(evTimeout3Message);});
 								stateVarsCopy.stateVartlcStateMachine = States::Operational; /* Default in entry chain  */
 								stateVarsCopy.stateVarOperational = States::OperationalRed; /* Default in entry chain  */

 								/* adjust state variables  */
 								stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn;
 							}else{
 								/* Intentionally left blank */
 							} /*end of event selection */
 						}
 					break; /* end of case OutOfService  */

 					case States::Operational:

 						switch (stateVars.stateVarOperational) {

 							case States::OperationalRed:
 								if(msg==TLCEvent::evTimeout3){
 									/* Transition from OperationalRed to OperationalRedYellow */
 									evConsumed = static_cast<int>(1U);
 									/* OnExit code of state OperationalRed */
 									{
 										resetRedLEDDef resetRedLED;
 										resetRedLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId3);

 									/* OnEntry code of state OperationalRedYellow */
 									{
 										setRedAndYellowLEDDef setRedAndYellowLED;
 										setRedAndYellowLED(*this);
 									}
 									
 									//Automatically generated from accept after transition
 									TLCEvent evTimeout4Message = TLCEvent::evTimeout4;
 									timeoutId4 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(1.0,"SI::second")), [this, evTimeout4Message](){onTimeout(evTimeout4Message);});

 									/* adjust state variables  */
 									stateVarsCopy.stateVarOperational = States::OperationalRedYellow;
 								}else{
 									/* Intentionally left blank */
 								} /*end of event selection */
 							break; /* end of case OperationalRed  */

 							case States::OperationalRedYellow:
 								if(msg==TLCEvent::evTimeout4){
 									/* Transition from OperationalRedYellow to OperationalGreen */
 									evConsumed = static_cast<int>(1U);
 									/* OnExit code of state OperationalRedYellow */
 									{
 										resetRedAndYellowLEDDef resetRedAndYellowLED;
 										resetRedAndYellowLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId4);

 									/* OnEntry code of state OperationalGreen */
 									{
 										setGreenLEDDef setGreenLED;
 										setGreenLED(*this);
 									}
 									
 									//Automatically generated from accept after transition
 									TLCEvent evTimeout5Message = TLCEvent::evTimeout5;
 									timeoutId5 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(1.0,"SI::second")), [this, evTimeout5Message](){onTimeout(evTimeout5Message);});

 									/* adjust state variables  */
 									stateVarsCopy.stateVarOperational = States::OperationalGreen;
 								}else{
 									/* Intentionally left blank */
 								} /*end of event selection */
 							break; /* end of case OperationalRedYellow  */

 							case States::OperationalGreen:
 								if(msg==TLCEvent::evTimeout5){
 									/* Transition from OperationalGreen to OperationalYellow */
 									evConsumed = static_cast<int>(1U);
 									/* OnExit code of state OperationalGreen */
 									{
 										resetGreenLEDDef resetGreenLED;
 										resetGreenLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId5);

 									/* OnEntry code of state OperationalYellow */
 									{
 										setYellowLEDDef setYellowLED;
 										setYellowLED(*this);
 									}
 									
 									//Automatically generated from accept after transition
 									TLCEvent evTimeout6Message = TLCEvent::evTimeout6;
 									timeoutId6 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(1.0,"SI::second")), [this, evTimeout6Message](){onTimeout(evTimeout6Message);});

 									/* adjust state variables  */
 									stateVarsCopy.stateVarOperational = States::OperationalYellow;
 								}else{
 									/* Intentionally left blank */
 								} /*end of event selection */
 							break; /* end of case OperationalGreen  */

 							case States::OperationalYellow:
 								if(msg==TLCEvent::evTimeout6){
 									/* Transition from OperationalYellow to OperationalRed */
 									evConsumed = static_cast<int>(1U);
 									/* OnExit code of state OperationalYellow */
 									{
 										resetYellowLEDDef resetYellowLED;
 										resetYellowLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId6);

 									/* OnEntry code of state OperationalRed */
 									{
 										setRedLEDDef setRedLED;
 										setRedLED(*this);
 									}
 									
 									//Automatically generated from accept after transition
 									TLCEvent evTimeout3Message = TLCEvent::evTimeout3;
 									timeoutId3 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(redtime,"SI::second")), [this, evTimeout3Message](){onTimeout(evTimeout3Message);});

 									/* adjust state variables  */
 									stateVarsCopy.stateVarOperational = States::OperationalRed;
 								}else{
 									/* Intentionally left blank */
 								} /*end of event selection */
 							break; /* end of case OperationalYellow  */

 							default:
 								/* Intentionally left blank */
 							break;
 						} /* end switch Operational */

 						/* Check if event was already processed  */
 						if(evConsumed==static_cast<int>(0)){

 							if(msg==TLCEvent::evError){
 								/* Transition from Operational to OutOfService */
 								evConsumed = static_cast<int>(1U);
 								
 								if(stateVars.stateVarOperational == States::OperationalRed){
 									{
 										resetRedLEDDef resetRedLED;
 										resetRedLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId3);


 								}else if(stateVars.stateVarOperational == States::OperationalRedYellow){
 									{
 										resetRedAndYellowLEDDef resetRedAndYellowLED;
 										resetRedAndYellowLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId4);


 								}else if(stateVars.stateVarOperational == States::OperationalGreen){
 									{
 										resetGreenLEDDef resetGreenLED;
 										resetGreenLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId5);


 								}else if(stateVars.stateVarOperational == States::OperationalYellow){
 									{
 										resetYellowLEDDef resetYellowLED;
 										resetYellowLED(*this);
 									}
 									
 									tm.cancelTimer(timeoutId6);

 								}else {
 									/* Intentionally left blank */
 								};


 								{
 									setYellowLEDDef setYellowLED;
 									setYellowLED(*this);
 								}
 								
 								//Automatically generated from accept after transition
 								TLCEvent evTimeout1Message = TLCEvent::evTimeout1;
 								timeoutId1 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout1Message](){onTimeout(evTimeout1Message);});
 								stateVarsCopy.stateVartlcStateMachine = States::OutOfService; /* Default in entry chain  */
 								stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn; /* Default in entry chain  */

 								/* adjust state variables  */
 								stateVarsCopy.stateVarOperational = States::OperationalRed;
 							}else{
 								/* Intentionally left blank */
 							} /*end of event selection */
 						}
 					break; /* end of case Operational  */

 					default:
 						/* Intentionally left blank */
 					break;
 				} /* end switch tlcStateMachine */
 			break; /* end of case tlcStateMachine  */

 			default:
 				/* Intentionally left blank */
 			break;
 		} /* end switch stateVar_root */

 		// Copy state variables back
 		stateVars = stateVarsCopy;

 	} // end state machine handler


};


	
class TrafficLightSystem : public Part {

public:


	TrafficLightSystem() {
	}



	

	std::unique_ptr<TrafficManagementCenter> tmc = std::make_unique<TrafficManagementCenter>();

	std::unique_ptr<TrafficLightController> tlc = std::make_unique<TrafficLightController>();


 	virtual void process() override {

		tmc->process();

		tlc->process();

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void) override{			
		assert(tmc);
		assert(tlc);
		tmc->init();

		tlc->init();
		
		
		connect(&tmc->sendPort,&tlc->recvPort);
 	}






};


} // end of namespace TrafficLight
