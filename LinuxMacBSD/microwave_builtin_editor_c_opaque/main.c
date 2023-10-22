#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include <termios.h>
#include <sys/time.h>
#include <curses.h>

static struct timespec tv={0,1000*1000*100}; // ~100ms

// parameter
T_PWR pwr = PWR_LOW;


// message to state machine
OVEN_EVENT_T msg;
WINDOW *win, *borderwindow, *logwin;     

int main(int argc, char* argv[]){

	unsigned char timer_status;
	
	struct OVEN_INSTANCEDATA *instData = malloc(ovenSizeOf());
	ovenInitMachine(instData,0U);

	win = initscr(); // new screen will be created
  nodelay(win, TRUE);
  noecho();
	scrollok(stdscr,TRUE);

	logwin = newwin(38, 78, 6, 3);
	borderwindow = newwin(40, 80, 5, 2);
	box(borderwindow, 0, 0);
	nodelay(logwin, TRUE);
	scrollok(logwin,TRUE);

	//
	// interactive mode
	//
	printw("Use keys: + for inc; - for dec; o for open and c for close.\n");
	printw("          'p' to set medium power; 'P' to set high power\n");		
	do{
		nanosleep(&tv,0);
		timer_status = tick();
		
		if(timer_status){
			msg=evTimeout;
			oven(instData,msg);	
		}

		char ch=getch();
		if(ch=='+'){
			msg=evInc;
		}else if(ch=='-'){
			msg=evDec;
		}else if(ch=='c'){
			msg=evDoorClosed;
		}else if(ch=='o'){
			msg=evDoorOpen;
		}else if(ch=='q'){
			endwin(); // must call
			exit(0);					
		}else if(ch=='P'){
			pwr = PWR_HIGH;
			msg=evPwr;
		}else if(ch=='p'){
			pwr = PWR_LOW;
			msg=evPwr;
		}else{
			msg=OVEN_NO_MSG;
		}
		oven(instData,msg);
		wrefresh(logwin);
		wrefresh(borderwindow);
	
	}while(1);


		
	return 0;
}
