/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p UModel -l cx -t MicrowaveOven:MachineClass -doxygen -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually  */
/* Generated on: Fri Aug 25 08:41:11 CEST 2023 / Version 6.0.5.3690 */


#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include "oven_selftest.h"
#include <stdio.h>

extern unsigned char msg;
extern T_PWR pwr;

uint8_t testResult;

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
structIdle->structCooking[minlen=1 label=<evDoorClo...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Idle Cooking
structCookingPause->structCooking[minlen=1 label=<evDoorClo...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)CookingPause Cooking
structCooking->structCompleted[minlen=1 label=<evTimeout...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Cooking Completed
structCompleted->structIdle[minlen=1 label=<evDoorOpe...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Completed Idle
structCooking->structCookingPause[minlen=1 label=<evDoorOpe...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)Cooking CookingPause
structDefaultActive->structIdle[minlen=1 label=<  > fontname=Arial; fontsize=14, style=dotted]; // (A) 
structDefaultroot->structUe0384d70-9ade-4a52-bc25-ad2e77ae479c[minlen=1 label=< &#47; testRes... > fontname=Arial; fontsize=14, style=dotted]; // (A) 

hidden0[shape=point height=0]; // G1
structDefaultActive -> hidden0[ weight = 1  dir=none, arrowhead=none, constraint=false, color=black, ltail=clusterActive]; // (G3)
hidden0 -> structIdle[ weight = 1 minlen=1 taillabel=<evPwr&#47; <br ALIGN="LEFT"/>o...>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterActive];   //G4  


hidden1[shape=point height=0]; // G1
structDefaultActive -> hidden1[ weight = 1  dir=none, arrowhead=none, constraint=false, color=black, ltail=clusterActive]; // (G3)
hidden1 -> structIdle[ weight = 1 minlen=1 taillabel=<evInc&#47; <br ALIGN="LEFT"/>t...>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterActive];   //G4  


hidden2[shape=point height=0]; // G1
structDefaultActive -> hidden2[ weight = 1  dir=none, arrowhead=none, constraint=false, color=black, ltail=clusterActive]; // (G3)
hidden2 -> structIdle[ weight = 1 minlen=1 taillabel=<evDec&#47; <br ALIGN="LEFT"/>t...>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterActive];   //G4  

structUe0384d70-9ade-4a52-bc25-ad2e77ae479c -> structIdle[minlen=1 labeldistance=2.0 , label=<&#91;else&#93;>  color=black, fontname=Arial; fontsize=14, fontcolor=black lhead=clusterActive];// (D4)Ue0384d70-9ade-4a52-bc25-ad2e77ae479c Active
structUe0384d70-9ade-4a52-bc25-ad2e77ae479c->structError[minlen=1 label=<&#91;testResu...>  color=black, fontname=Arial; fontsize=14, fontcolor=black]; // (B)null Error



subgraph "cluster0"{ style=invis;label=""
subgraph "cluster1"{ style=invis;label=""
subgraph "cluster2"{ style=invis;label=""
subgraph "cluster3"{ style=invis;label=""
subgraph "cluster4"{ style=invis;label=""
subgraph "cluster5"{ style=invis;label=""
subgraph "cluster6"{ style=invis;label=""
subgraph "cluster7"{ style=invis;label=""
subgraph "clusterActive"{ margin=10 fontname=Arial; fontsize=14; fontsize=14
color=black; style=rounded;
 label=< <TABLE BORDER="0" ><TR><TD PORT="W"></TD><TD PORT="N"> Active(H) </TD><TD PORT="E"></TD></TR><HR/><TR><TD> </TD></TR>
</TABLE> >;

structIdle[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Idle</TD><TD PORT="E"></TD></TR>
<TR><TD COLSPAN="3" ALIGN="LEFT">Entry:<br ALIGN="LEFT"/>ovenOff();<br ALIGN="LEFT"/></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structCooking[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Cooking</TD><TD PORT="E"></TD></TR>
<TR><TD COLSPAN="3" ALIGN="LEFT">Entry:<br ALIGN="LEFT"/>ovenOn();<br ALIGN="LEFT"/></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structCookingPause[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">CookingPause</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];


structCompleted[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Completed</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];

structDefaultActive[shape=circle, fontsize=14, height=0.3, width=0.3, fixedsize=true, fontname=Arial; fontsize=14, fontcolor=white,fillcolor=black,  style=filled label=< <TABLE BORDER="0"><TR><TD PORT="NW"></TD> <TD PORT="N"></TD> <TD PORT="NE"></TD></TR><TR><TD PORT="W"> </TD> <TD BORDER="0" ALIGN="CENTER"> <FONT COLOR="white">(H)</FONT>  </TD> <TD PORT="E"></TD></TR><TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR></TABLE> >];};
};
};
};
};
};
};
};
};

structDefaultroot[shape=circle, fontsize=14, height=0.3, width=0.3, fixedsize=true, fontname=Arial; fontsize=14, fontcolor=white,fillcolor=black,  style=filled label=< <TABLE BORDER="0"><TR><TD PORT="NW"></TD> <TD PORT="N"></TD> <TD PORT="NE"></TD></TR><TR><TD PORT="W"> </TD> <TD BORDER="0" ALIGN="CENTER"> <FONT COLOR="white"> </FONT>  </TD> <TD PORT="E"></TD></TR><TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR></TABLE> >];structUe0384d70-9ade-4a52-bc25-ad2e77ae479c [label = "" xlabel="" fontcolor=black  shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=Arial; fontsize=14 color=black];

structError[margin="0.4,0.08" penwidth="1" style="rounded" BORDER="1" color="black" fontname=Arial; fontsize=14 label=< <TABLE STYLE="rounded" BORDER="0" CELLBORDER="0">
<TR><TD PORT="NW"></TD><TD PORT="N"></TD><TD PORT="NE"></TD></TR>
<TR><TD PORT="W"></TD> <TD BORDER="1" SIDES="B">Error</TD><TD PORT="E"></TD></TR>
<TR><TD PORT="SW"></TD><TD PORT="S"></TD><TD PORT="SE"></TD></TR>
</TABLE> >];

structUe0384d70-9ade-4a52-bc25-ad2e77ae479c [label = "" xlabel="" fontcolor=black  shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=Arial; fontsize=14 color=black];
}
\enddot
*/




void  oven(OVEN_INSTANCEDATA_T *instanceVar){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->activeEntry==1U){
		testResult = selftest();
		if(testResult>0){
			instanceVar->stateVar = Error;
		}else{
			instanceVar->stateVar = Active;
		ovenOff();
		
		}

		instanceVar->activeEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case Active:

			switch (instanceVar->stateVarActive) {

				case Idle:
					if(msg==(OVEN_EVENT_T)evDoorClosed){
						if(timer_preset()>0){
							/* Transition from Idle to Cooking */
							evConsumed=1U;

							/* Action code for transition  */
							timer_start();


							/* OnEntry code of state Cooking */
							ovenOn();

							/* adjust state variables  */
							instanceVar->stateVarActive = Cooking;
						}else{
							/* Intentionally left blank */
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
						instanceVar->stateVarActive = CookingPause;
					}else if(msg==(OVEN_EVENT_T)evTimeout){
						/* Transition from Cooking to Completed */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_clear();



						/* adjust state variables  */
						instanceVar->stateVarActive = Completed;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case CookingPause:
					if(msg==(OVEN_EVENT_T)evDoorClosed){
						/* Transition from CookingPause to Cooking */
						evConsumed=1U;

						/* Action code for transition  */
						timer_cont();


						/* OnEntry code of state Cooking */
						ovenOn();

						/* adjust state variables  */
						instanceVar->stateVarActive = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				case Completed:
					if(msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1U;

						/* OnEntry code of state Idle */
						ovenOff();

						/* adjust state variables  */
						instanceVar->stateVarActive = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Active */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(OVEN_EVENT_T)evDec){
					/* Transition from Active to Active */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_dec();


					instanceVar->stateVar = Active;/* entry chain  */
					if(instanceVar->stateVarActive==Idle){
						ovenOff();

					}else if(instanceVar->stateVarActive==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(OVEN_EVENT_T)evInc){
					/* Transition from Active to Active */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_inc();


					instanceVar->stateVar = Active;/* entry chain  */
					if(instanceVar->stateVarActive==Idle){
						ovenOff();

					}else if(instanceVar->stateVarActive==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(OVEN_EVENT_T)evPwr){
					/* Transition from Active to Active */
					evConsumed=1U;
					
					/* Action code for transition  */
					ovenSetPwr(pwr);


					instanceVar->stateVar = Active;/* entry chain  */
					if(instanceVar->stateVarActive==Idle){
						ovenOff();

					}else if(instanceVar->stateVarActive==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Active  */

		case Error:
		break; /* end of case Error  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar){
	instanceVar->activeEntry = 1U;
	instanceVar->stateVar = Active;
	instanceVar->stateVarActive = Idle;

}
/* Helper(s) to reset history */
void ovenResetHistoryActive(OVEN_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarActive=Idle;
}


/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInActive(const OVEN_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarActive== Completed)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInError(const OVEN_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== Error)) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarActive== Cooking)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarActive== Idle)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarActive== CookingPause)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar){
	OVEN_STATES_T innermost;

	if(ovenIsInCookingPause(instanceVar)){
		innermost =CookingPause;
	}else if(ovenIsInIdle(instanceVar)){
		innermost = Idle;
	}else if(ovenIsInCooking(instanceVar)){
		innermost = Cooking;
	}else if(ovenIsInCompleted(instanceVar)){
		innermost = Completed;
	}else if(ovenIsInError(instanceVar)){
		innermost = Error;
	}else{
		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;}

