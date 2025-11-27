/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

// Command line options: -d -l cppx -p sysml2text -o system tl.sysml 
// This file is generated from tl.sysml - do not edit manually
// Generated on: Thu Nov 27 18:31:04 CET 2025 / 7.0


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

#include "TrafficManagementCenterSM_ext.h"

#include "TrafficLightControllerSM_ext.h"

namespace TrafficLight{
struct ControlPortDataDef {
    TRAFFICLIGHTCONTROLLERSM_EVENT_T msg;
    int test;
};

using ControlPortData  = std::variant<ControlPortDataDef>;
	
	

class Test : public Part {

public:


	Test() {
	}

	TimeoutManager tm;


	//elements of this part	



 	virtual void process() override {

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void){

 	}


};


class TrafficManagementCenter : public Part {

public:


	TrafficManagementCenter() {
	}

	TimeoutManager tm;



	Port<ControlPortData> sendPort;


	//elements of this part	

	TimeoutManager::TimeoutHandle timeoutId0  = -1 ;


	std::unique_ptr<Test> test1;

    virtual void onTimeout(const TRAFFICMANAGEMENTCENTERSM_EVENT_T & event) {assert(false);} ;

 	virtual void process() override {

		test1->process();
	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void){
		assert(test1);
		test1->init();

 	}


};


class TrafficLightController : public Part {

public:


	TrafficLightController() {
		recvPort.addHandler([this](const ControlPortData& e){
			std::visit([this](auto&& ev) { handle(ev); }, e);
		});

	}

	TimeoutManager tm;

	// called by process() to handle received event
	virtual void handle(const ControlPortDataDef& e) {assert(false);};



	InputPort<ControlPortData> recvPort;


	//elements of this part	

	int redtime  = 1 ;
	TimeoutManager::TimeoutHandle timeoutId1  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId2  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId3  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId4  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId5  = -1 ;
	TimeoutManager::TimeoutHandle timeoutId6  = -1 ;


	std::unique_ptr<Test> test1;

    virtual void onTimeout(const TRAFFICLIGHTCONTROLLERSM_EVENT_T & event) {assert(false);} ;

 	virtual void process() override {
		recvPort.process();


		test1->process();
	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void){
		assert(test1);
		test1->init();

 	}

 	virtual void setRedLED(void){std::cout << "debug: setRedLED() method executed" << std::endl;};

 	virtual void resetRedLED(void){std::cout << "debug: resetRedLED() method executed" << std::endl;};

 	virtual void setYellowLED(void){std::cout << "debug: setYellowLED() method executed" << std::endl;};

 	virtual void resetYellowLED(void){std::cout << "debug: resetYellowLED() method executed" << std::endl;};

 	virtual void setGreenLED(void){std::cout << "debug: setGreenLED() method executed" << std::endl;};

 	virtual void resetGreenLED(void){std::cout << "debug: resetGreenLED() method executed" << std::endl;};

 	virtual void setRedAndYellowLED(void){std::cout << "debug: setRedAndYellowLED() method executed" << std::endl;};

 	virtual void resetRedAndYellowLED(void){std::cout << "debug: resetRedAndYellowLED() method executed" << std::endl;};



};


class TrafficLightSystem : public Part {

public:


	TrafficLightSystem() {
	}

	TimeoutManager tm;


	//elements of this part	


	std::unique_ptr<TrafficManagementCenter> tmc;
	std::unique_ptr<TrafficLightController> tlc;

 	virtual void process() override {

		tmc->process();
		tlc->process();
	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void){
		assert(tmc);
		assert(tlc);
		tmc->init();
		tlc->init();

		connect(&tmc->sendPort,&tlc->recvPort);
 	}


};

} // end of namespace TrafficLight
