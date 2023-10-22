//
// (C) Peter Mueller, sinelabore.com
//  
// This simple program simulates a microwave oven.
// The oven is based on a state machine modelled in Cadifra UML editor.
// The code from the UML model is fully generated.
// The main function reads the keyboard and sends events to the state machine.
// A simple "timer" is used as time base.
//
// To built this code install mingw on Windows and call make.
//

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <windows.h> 
#include <conio.h> 
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"


OVEN_INSTANCEDATA_T instData = OVEN_INSTANCEDATA_INIT;

// parameter
T_PWR pwr = PWR_LOW;

// message to state machine
OVEN_EVENT_T msg;

       
int main(void){

	unsigned char timer_status;
	
	//
	// interactive mode
	//
	//init_keyboard();
	printf("Unknown key. Use + for inc; - for dec; o for open and c for close.\n");
	printf("             Use 'p' to set medium power; 'P' to set high power\n");		
	do{
		Sleep(100); // sleep 100ms
		timer_status = tick();
		
		if(timer_status){
			msg=evTimeout;
		}else if(_kbhit()){
			int ch=_getch();
			if(ch=='+')
				msg=evInc;
			else if(ch=='-')
				msg=evDec;
			else if(ch=='c')
				msg=evDoorClosed;
			else if(ch=='o')
				msg=evDoorOpen;
			else if(ch=='q'){
				exit(0);					
			}else if(ch=='P'){
				pwr = PWR_HIGH;
				msg=evPwr;
			}else if(ch=='p'){
				pwr = PWR_LOW;
				msg=evPwr;
			}else{
				printf("Unknown key. Use + for inc; - for dec; o for open and c for close.\n");
				printf("             Use 'p' to set medium power; 'P' to set high power\n");
				msg=OVEN_NO_MSG;
			}
		}else{
			msg=OVEN_NO_MSG;
		}
			
		oven(&instData);			
	
	}while(1);
	
	return 0;
}
