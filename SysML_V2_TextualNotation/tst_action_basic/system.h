/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

// Command line options: -d -l cppx -p sysml2text -o system test.sysml 
// This file is generated from test.sysml - do not edit manually
// Generated on: Mon Mar 16 19:54:03 CET 2026 / 7.1


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

namespace Test{
	
class AParent : public Part {

public:


	AParent() {
	}



	//elements of this part	

	int anAttributeOfAParent  = 0 ;


	


 	virtual void process() override {

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void) override{			
		
 	}


 	// State machine initialize function. Must be called once before event processing.

 	// Debug support functions

 	// Reporting functions

 	// State machine handler function

};


	
class A : public AParent {

public:


	A() {
	}



	//elements of this part	

	int anAttributeOfA  = 0 ;


	


 	virtual void process() override {

	}

	// must be called by derived classes after constructing the parts
 	virtual void init(void) override{			
		
 	}


 	struct ParentActionDef {
 		// Top-level activity: parent is the part
 		A* part_ptr_ = nullptr;
 		A& getPart() { assert(part_ptr_); return *part_ptr_; }

 		virtual void operator()(A& parent_){
 			std::cout << "debug: ParentAction.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;

 			struct ParentActionContext{
 				//owning part
 				A& parent;
 				//indirect accessible features
 				int& anAttributeOfA;
 				int& anAttributeOfAParent;

 				ParentActionContext(A& parent_ 
 					) : parent(parent_)
 					,
 					anAttributeOfA(parent_.anAttributeOfA),
 					anAttributeOfAParent(parent_.anAttributeOfAParent){}
 			}ctx{parent_};

 			//make context available
 			auto& anAttributeOfA = ctx.anAttributeOfA;
 			auto& anAttributeOfAParent = ctx.anAttributeOfAParent;

 		} // end run method
 		virtual ~ParentActionDef() = default;

 	};//ParentAction


 	struct ParentWithParaDef {
 		// Top-level activity: parent is the part
 		A* part_ptr_ = nullptr;
 		A& getPart() { assert(part_ptr_); return *part_ptr_; }


 		int test  = 0 ;

 		double x; // in
 		double p; // out or inout
 		virtual void operator()(A& parent_){
 			std::cout << "debug: ParentWithPara.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;



 			enum class ParentWithParaID {

 				assignmentNode0, 
 				ImplicitStartNode0, 
 				assignmentNode1, 
 				assignmentNode2,
 				__END__
 			};


 			ParentWithParaID id = ParentWithParaID::assignmentNode0;

 			struct ParentWithParaContext{
 				//owning part
 				A& parent;
 				//indirect accessible features
 				int& anAttributeOfA;
 				int& anAttributeOfAParent;

 				ParentWithParaContext(A& parent_ 
 					) : parent(parent_)
 					,
 					anAttributeOfA(parent_.anAttributeOfA),
 					anAttributeOfAParent(parent_.anAttributeOfAParent){}
 			}ctx{parent_};

 			//make context available
 			auto& anAttributeOfA = ctx.anAttributeOfA;
 			auto& anAttributeOfAParent = ctx.anAttributeOfAParent;


 			while(id !=ParentWithParaID::__END__ ){ 
 				switch(id) {
 					case ParentWithParaID::assignmentNode0:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = ParentWithParaID::__END__;
 						p = x*x;
 						id = ParentWithParaID::assignmentNode1;
 						break;

 					case ParentWithParaID::assignmentNode1:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = ParentWithParaID::__END__;
 						anAttributeOfA = p;
 						id = ParentWithParaID::assignmentNode2;
 						break;

 					case ParentWithParaID::assignmentNode2:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = ParentWithParaID::__END__;
 						anAttributeOfAParent = p;
 						break;

 					default:
 						break;

 				} //end switch
 			} //end while
 		} // end run method
 		virtual ~ParentWithParaDef() = default;

 	};//ParentWithPara


 	struct runDef {
 		// Top-level activity: parent is the part
 		A* part_ptr_ = nullptr;
 		A& getPart() { assert(part_ptr_); return *part_ptr_; }


 		double x  = 0.0 ;

 		ParentActionDef act1{};


 		ParentWithParaDef act2{};



 		struct act3Def {
 			// Nested activity
 			runDef* parent_action_ptr_ = nullptr;
 			runDef& getParentAction() { assert(parent_action_ptr_); return *parent_action_ptr_; }
 			double n; // in
 			double u; // out or inout
 			virtual void operator()(runDef& parent_){
 				std::cout << "debug: act3.operator() method executed" << std::endl;
 				parent_action_ptr_ = &parent_;



 				enum class runID {

 					assignmentNode3, 
 					ImplicitStartNode0,
 					__END__
 				};


 				runID id = runID::assignmentNode3;

 				struct act3Context{
 					//owning part
 					runDef& parent;
 					//indirect accessible features
 					double& x;
 					int& anAttributeOfA;
 					int& anAttributeOfAParent;

 					act3Context(runDef& parent_ 
 						) : parent(parent_)
 						,
 						x(parent_.x),
 						anAttributeOfA(parent_.getPart().anAttributeOfA),
 						anAttributeOfAParent(parent_.getPart().anAttributeOfAParent){}
 				}ctx{parent_};

 				//make context available
 				auto& x = ctx.x;
 				auto& anAttributeOfA = ctx.anAttributeOfA;
 				auto& anAttributeOfAParent = ctx.anAttributeOfAParent;

 				while(id !=runID::__END__ ){ 
 					switch(id) {
 						case runID::assignmentNode3:
 							// End of the action by default to prevent endless loops if user has not set end nodes
 							id = runID::__END__;
 							u = 8;
 							break;

 						default:
 							break;

 					} //end switch
 				} //end while
 			} // end run method
 			virtual ~act3Def() = default;

 		}act3; // end activity act3Def




 		struct Path1Def {
 			// Nested activity
 			runDef* parent_action_ptr_ = nullptr;
 			runDef& getParentAction() { assert(parent_action_ptr_); return *parent_action_ptr_; }

 			virtual void operator()(runDef& parent_){
 				std::cout << "debug: Path1.operator() method executed" << std::endl;
 				parent_action_ptr_ = &parent_;

 				struct Path1Context{
 					//owning part
 					runDef& parent;
 					//indirect accessible features
 					double& x;
 					int& anAttributeOfA;
 					int& anAttributeOfAParent;

 					Path1Context(runDef& parent_ 
 						) : parent(parent_)
 						,
 						x(parent_.x),
 						anAttributeOfA(parent_.getPart().anAttributeOfA),
 						anAttributeOfAParent(parent_.getPart().anAttributeOfAParent){}
 				}ctx{parent_};

 				//make context available
 				auto& x = ctx.x;
 				auto& anAttributeOfA = ctx.anAttributeOfA;
 				auto& anAttributeOfAParent = ctx.anAttributeOfAParent;

 			} // end run method
 			virtual ~Path1Def() = default;

 		}Path1; // end activity Path1Def




 		struct Path2Def {
 			// Nested activity
 			runDef* parent_action_ptr_ = nullptr;
 			runDef& getParentAction() { assert(parent_action_ptr_); return *parent_action_ptr_; }

 			virtual void operator()(runDef& parent_){
 				std::cout << "debug: Path2.operator() method executed" << std::endl;
 				parent_action_ptr_ = &parent_;

 				struct Path2Context{
 					//owning part
 					runDef& parent;
 					//indirect accessible features
 					double& x;
 					int& anAttributeOfA;
 					int& anAttributeOfAParent;

 					Path2Context(runDef& parent_ 
 						) : parent(parent_)
 						,
 						x(parent_.x),
 						anAttributeOfA(parent_.getPart().anAttributeOfA),
 						anAttributeOfAParent(parent_.getPart().anAttributeOfAParent){}
 				}ctx{parent_};

 				//make context available
 				auto& x = ctx.x;
 				auto& anAttributeOfA = ctx.anAttributeOfA;
 				auto& anAttributeOfAParent = ctx.anAttributeOfAParent;

 			} // end run method
 			virtual ~Path2Def() = default;

 		}Path2; // end activity Path2Def




 		struct act1_1Def {
 			// Nested activity
 			runDef* parent_action_ptr_ = nullptr;
 			runDef& getParentAction() { assert(parent_action_ptr_); return *parent_action_ptr_; }

 			virtual void operator()(runDef& parent_){
 				std::cout << "debug: act1_1.operator() method executed" << std::endl;
 				parent_action_ptr_ = &parent_;

 				struct act1_1Context{
 					//owning part
 					runDef& parent;
 					//indirect accessible features
 					double& x;
 					int& anAttributeOfA;
 					int& anAttributeOfAParent;

 					act1_1Context(runDef& parent_ 
 						) : parent(parent_)
 						,
 						x(parent_.x),
 						anAttributeOfA(parent_.getPart().anAttributeOfA),
 						anAttributeOfAParent(parent_.getPart().anAttributeOfAParent){}
 				}ctx{parent_};

 				//make context available
 				auto& x = ctx.x;
 				auto& anAttributeOfA = ctx.anAttributeOfA;
 				auto& anAttributeOfAParent = ctx.anAttributeOfAParent;

 			} // end run method
 			virtual ~act1_1Def() = default;

 		}act1_1; // end activity act1_1Def



 		double n; // in
 		double p; // out or inout
 		double u; // out or inout
 		virtual void operator()(A& parent_){
 			std::cout << "debug: run.operator() method executed" << std::endl;
 			part_ptr_ = &parent_;



 			enum class runID {

 				act1, 
 				act2, 
 				act3, 
 				Path1, 
 				Terminal0, 
 				Path2, 
 				Terminal1, 
 				start, 
 				assignmentNode4, 
 				assignmentNode5, 
 				act1_1, 
 				assignmentNode6, 
 				assignmentNode7, 
 				d1, 
 				Terminal2,
 				__END__
 			};


 			runID id = runID::assignmentNode4;

 			struct runContext{
 				//owning part
 				A& parent;
 				//indirect accessible features
 				int& anAttributeOfA;
 				int& anAttributeOfAParent;

 				runContext(A& parent_ 
 					) : parent(parent_)
 					,
 					anAttributeOfA(parent_.anAttributeOfA),
 					anAttributeOfAParent(parent_.anAttributeOfAParent){}
 			}ctx{parent_};

 			//make context available
 			auto& anAttributeOfA = ctx.anAttributeOfA;
 			auto& anAttributeOfAParent = ctx.anAttributeOfAParent;


 			while(id !=runID::__END__ ){ 
 				switch(id) {
 					case runID::act1:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						act1(parent_);
 						id = runID::assignmentNode5;
 						break;

 					case runID::act2:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						act2.x = x;
 						act2(parent_);
 						id = runID::assignmentNode6;
 						break;

 					case runID::act3:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						act3.n = n;
 						act3(*this);
 						id = runID::assignmentNode7;
 						break;

 					case runID::Path1:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						Path1(*this);
 						id = runID::Terminal0;
 						break;

 					case runID::Terminal0:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						id = runID::__END__;
 						break;

 					case runID::Path2:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						Path2(*this);
 						id = runID::Terminal1;
 						break;

 					case runID::Terminal1:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						id = runID::__END__;
 						break;

 					case runID::assignmentNode4:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						x = n;
 						id = runID::act1;
 						break;

 					case runID::assignmentNode5:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						x = 2;
 						id = runID::act1_1;
 						break;

 					case runID::act1_1:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						act1_1(*this);
 						id = runID::act2;
 						break;

 					case runID::assignmentNode6:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						p = act2.p;
 						id = runID::act3;
 						break;

 					case runID::assignmentNode7:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						u = act3.u;
 						id = runID::d1;
 						break;

 					case runID::d1:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						if(act2.p==4){
 							id = runID::Path2;
 						}else if(n==1){
 							id = runID::Path1;
 						}else{
 							id = runID::Terminal2;
 						}
 						break;

 					case runID::Terminal2:
 						// End of the action by default to prevent endless loops if user has not set end nodes
 						id = runID::__END__;
 						id = runID::__END__;
 						break;

 					default:
 						break;

 				} //end switch
 			} //end while
 		} // end run method
 		virtual ~runDef() = default;

 	};//run



 	// State machine initialize function. Must be called once before event processing.

 	// Debug support functions

 	// Reporting functions

 	// State machine handler function

};


} // end of namespace Test
