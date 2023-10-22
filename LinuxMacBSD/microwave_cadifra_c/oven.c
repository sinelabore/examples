/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p CADIFRA -l cx -doxygen -o oven first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually  */
/* Generated on: Sun Oct 22 17:56:02 CEST 2023 / Version 6.1.3706 */


#include <stdint.h> // std. types
#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include <stdio.h>

extern OVEN_EVENT_T msg;
extern T_PWR pwr;
uint8_t status;

uint8_t initHardware(void){
   return 0; // just as demo
}

/**Here is the graph that shows the state machine this function implements\dot
digraph G {
rankdir=TB;
bgcolor=transparent;
node[fontsize=14; fontname=Arial; fontsize=14; shape=box];
edge[fontsize=14; fontname=Arial; fontsize=14; arrowsize=0.7];
compound=true;
remincross=true;
nodesep=0.5;
ranksep=0.5;
structDefaultroot->structC1[minlen=1 label=< &#47; status=... > fontname=Arial; fontsize=14, style=dotted]; // (A) 

hidden0[shape=point height=0]; // G1
structDefaultSuper -> hidden0[ weight = 1  dir=none, arrowhead=none, constraint=false, color=black, ltail=clusterSuper]; // (G3)
hidden0 -> structIdle[ weight = 1 minlen=1 taillabel=<evPwr&#47; <br ALIGN="LEFT"/>o...>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterSuper];   //G4  

structIdle->structC2[minlen=1 label=<evDoorClo...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Idle C2
structC2->structIdle[minlen=1 label=<&#91;timer_pr...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)C2 Idle
structC1 -> structIdle[minlen=1 labeldistance=2.0 , label=<&#91;else&#93;>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterSuper];// (D4)C1 Super
structC1->structError[minlen=1 label=<&#91;status!=...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)C1 Error
structC2->structCooking[minlen=1 label=<&#91;else&#93;&#47; <br ALIGN="LEFT"/>...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)C2 Cooking
structCooking->structCookingPause[minlen=1 label=<evDoorOpe...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Cooking CookingPause
structCookingPause->structCooking[minlen=1 label=<evDoorClo...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)CookingPause Cooking
structCooking->structCompleted[minlen=1 label=<evTimeout...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Cooking Completed
structCompleted->structIdle[minlen=1 label=<evDoorOpe...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Completed Idle
structDefaultSuper->structIdle[minlen=1 label=< &#47; ovenOff... > fontname=Arial; fontsize=14, style=dotted]; // (A) 

hidden1[shape=point height=0]; // G1
structDefaultSuper -> hidden1[ weight = 1  dir=none, arrowhead=none, constraint=false, color=black, ltail=clusterSuper]; // (G3)
hidden1 -> structIdle[ weight = 1 minlen=1 taillabel=<evInc&#47; <br ALIGN="LEFT"/>t...>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterSuper];   //G4  


hidden2[shape=point height=0]; // G1
structDefaultSuper -> hidden2[ weight = 1  dir=none, arrowhead=none, constraint=false, color=black, ltail=clusterSuper]; // (G3)
hidden2 -> structIdle[ weight = 1 minlen=1 taillabel=<evDec&#47; <br ALIGN="LEFT"/>t...>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterSuper];   //G4  

structDefaultroot[shape=circle, fontsize=14, height=0.3, width=0.3, fixedsize=true, fontname=Arial; fontsize=14, fontcolor=white,fillcolor=black,  style=filled label=< <TABLE BORDER="0"><TR><TD PORT="NW"></TD> <TD PORT="N"></TD> <TD PORT="NE"></TD></TR><TR><TD PORT="W"> </TD> <TD BORDER="0" ALIGN="CENTER"> <FONT COLOR="white"> </FONT>  </TD> <TD PORT="E"></TD></TR><TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR></TABLE> >];structC1 [label = "" xlabel="C1" fontcolor=black  shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=Arial; fontsize=14 color=black];

structError[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Error</TD><TD PORT="E"></TD></TR>
<TR><TD COLSPAN="3" ALIGN="LEFT">Entry:<br ALIGN="LEFT"/>&#47;&#47; signal ...<br ALIGN="LEFT"/></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];




subgraph "cluster0"{ style=invis;label=""
subgraph "cluster1"{ style=invis;label=""
subgraph "cluster2"{ style=invis;label=""
subgraph "cluster3"{ style=invis;label=""
subgraph "cluster4"{ style=invis;label=""
subgraph "cluster5"{ style=invis;label=""
subgraph "cluster6"{ style=invis;label=""
subgraph "cluster7"{ style=invis;label=""
subgraph "clusterSuper"{ margin=10 fontname=Arial; fontsize=14; fontsize=14
color=black; style=rounded;
 label=< <TABLE BORDER="0" ><TR><TD PORT="W"></TD><TD PORT="N"> Super(H) </TD><TD PORT="E"></TD></TR><HR/><TR><TD> </TD></TR>
</TABLE> >;

structIdle[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Idle</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];
structC2 [label = "" xlabel="C2" fontcolor=black  shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=Arial; fontsize=14 color=black];


structCooking[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Cooking</TD><TD PORT="E"></TD></TR>
<TR><TD COLSPAN="3" ALIGN="LEFT">Entry:<br ALIGN="LEFT"/>ovenOn();<br ALIGN="LEFT"/></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structCompleted[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Completed</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structCookingPause[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">CookingPause</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];

structDefaultSuper[shape=circle, fontsize=14, height=0.3, width=0.3, fixedsize=true, fontname=Arial; fontsize=14, fontcolor=white,fillcolor=black,  style=filled label=< <TABLE BORDER="0"><TR><TD PORT="NW"></TD> <TD PORT="N"></TD> <TD PORT="NE"></TD></TR><TR><TD PORT="W"> </TD> <TD BORDER="0" ALIGN="CENTER"> <FONT COLOR="white">(H)</FONT>  </TD> <TD PORT="E"></TD></TR><TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR></TABLE> >];};
};
};
};
};
};
};
};
};

structC1 [label = "" xlabel="C1" fontcolor=black  shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=Arial; fontsize=14 color=black];
}
\enddot
*/




void  oven(OVEN_INSTANCEDATA_T *instanceVar){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->superEntry==1U){
		status=initHardware();
		if(status!=0){
			// signal error
			
			instanceVar->stateVar = Error;
		}else{
			instanceVar->stateVar = Super;
		ovenOff();
		}

		instanceVar->superEntry=0U;
	}

	/* action code */
	/* just a comment */


	switch (instanceVar->stateVar) {

		case Error:
		break; /* end of case Error  */

		case Super:

			switch (instanceVar->stateVarSuper) {

				case Idle:
					if(msg==(OVEN_EVENT_T)evDoorClosed){
						if(timer_preset()==0){
							/* Transition from Idle to Idle */
							evConsumed=1U;


							/* adjust state variables  */
							instanceVar->stateVarSuper = Idle;
						}else{
							/* Transition from Idle to Cooking */
							evConsumed=1U;

							/* Action code for transition  */
							
							timer_start();


							/* OnEntry code of state Cooking */
							ovenOn();

							/* adjust state variables  */
							instanceVar->stateVarSuper = Cooking;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Idle  */

				case Cooking:
					if(msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Cooking to CookingPause */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_pause();



						/* adjust state variables  */
						instanceVar->stateVarSuper = CookingPause;
					}else if(msg==(OVEN_EVENT_T)evTimeout){
						/* Transition from Cooking to Completed */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_clear();



						/* adjust state variables  */
						instanceVar->stateVarSuper = Completed;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case Completed:
					if(msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1U;


						/* adjust state variables  */
						instanceVar->stateVarSuper = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				case CookingPause:
					if(msg==(OVEN_EVENT_T)evDoorClosed){
						/* Transition from CookingPause to Cooking */
						evConsumed=1U;

						/* Action code for transition  */
						timer_cont();


						/* OnEntry code of state Cooking */
						ovenOn();

						/* adjust state variables  */
						instanceVar->stateVarSuper = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Super */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(OVEN_EVENT_T)evDec){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_dec();


					instanceVar->stateVar = Super;/* entry chain  */
					if(instanceVar->stateVarSuper==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(OVEN_EVENT_T)evInc){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_inc();


					instanceVar->stateVar = Super;/* entry chain  */
					if(instanceVar->stateVarSuper==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(OVEN_EVENT_T)evPwr){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					ovenSetPwr(pwr);


					instanceVar->stateVar = Super;/* entry chain  */
					if(instanceVar->stateVarSuper==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Super  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Initialize state machine */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

}
/* Helper(s) to reset history */
void ovenResetHistorySuper(OVEN_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarSuper=Idle;
}


/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInError(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Error)) ? (1U) : (0U));
}

uint8_t ovenIsInSuper(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Idle)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Cooking)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Completed)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== CookingPause)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar) {
	OVEN_STATES_T innermost;

	if(ovenIsInCookingPause(instanceVar)) {
		innermost =CookingPause;
	}else if(ovenIsInCompleted(instanceVar)) {
		innermost = Completed;
	}else if(ovenIsInCooking(instanceVar)) {
		innermost = Cooking;
	}else if(ovenIsInIdle(instanceVar)) {
		innermost = Idle;
	}else if(ovenIsInError(instanceVar)) {
		innermost = Error;
	}else{
		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;	
}

