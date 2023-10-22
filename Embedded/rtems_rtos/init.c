
#include <stdio.h>
#include <bsp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <rtems/rtems_bsdnet.h>
#include "testmacros.h"
#include "mydefs.h"
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"

rtems_id   Task_id[ 4 ];         /* array of task ids */
rtems_name Task_name[ 4 ];       /* array of task names */
rtems_id   Queue_id;
uint8_t	   pwr=PWR_LOW;

OVEN_EVENT_T testpattern[] = {OVEN_NO_MSG,evDoorClosed,evPwr,evInc,evInc,evInc,evDoorClosed,OVEN_NO_MSG,OVEN_NO_MSG, \
OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,evDec,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,\
OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG,OVEN_NO_MSG};

static struct termios init_tio;

/*
  * Network configuration
  */
#define NETWORK_TASK_PRIORITY	50

int rtems_bsdnet_loopattach(struct rtems_bsdnet_ifconfig *);
static struct rtems_bsdnet_ifconfig loopdriver_config = {
	"lo0",		          /* name */
	rtems_bsdnet_loopattach,  /* attach function */
	NULL,			  /* link to next interface */
	"127.0.0.1",		  /* IP address */
	"255.0.0.0",		  /* IP net mask */
	NULL,                     /* Driver supplies hardware address */
	0			  /* Use default driver parameters */
};

struct rtems_bsdnet_config rtems_bsdnet_config = {
	&loopdriver_config,	/* Network interface */
	0,  /* Not use BOOTP to get network configuration */
	NETWORK_TASK_PRIORITY,  /* Network task priority */
	256*1024,		/* MBUF space */
	512*1024,		/* MBUF cluster space */

};

void init_keyboard()
{            
    tcgetattr(0,&init_tio);
}
void close_keyboard()
{            
    tcsetattr(0,TCSANOW,&init_tio);
}

int kbhit()
{
    struct termios tio;
    struct timeval tv;
    fd_set rfds;
    // set up terminal
    memcpy(&tio,&init_tio,sizeof(struct termios));
    tio.c_lflag &= ~(ICANON);
    tcsetattr(0,TCSANOW,&tio);
    // do not wait
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    tv.tv_usec = 0;
    tv.tv_sec  = 0;
    select(1,&rfds,NULL,NULL,&tv);
    // back to initial terminal mode
    tcsetattr(0,TCSANOW,&init_tio);
    return (FD_ISSET(0,&rfds)?1:0);
}

int readch()
{
    int ch;
    struct termios tio;
    // set up terminal
    memcpy(&tio,&init_tio,sizeof(struct termios));
    tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&tio);
    // input key
    read(0,&ch,1);
    // back to initial terminal mode
    tcsetattr(0,TCSANOW,&init_tio);
    return ch;
}

rtems_task oven_task(rtems_task_argument unused)
{
	OVEN_INSTANCEDATA_T inst = OVEN_INSTANCEDATA_INIT;

  for ( ; ; ) {
  	// returns if one event was processed
		oven(&inst);
  }
}

rtems_task Init(
  rtems_task_argument argument
)
{
	T_TIMER_STATE timer_status;
	OVEN_EVENT_T msg;

  rtems_status_code status;
  puts( "\n\n*** Oven Example ***" );
	
	status = rtems_bsdnet_initialize_network ();
	if ( status != RTEMS_SUCCESSFUL )
	 	fputs( "Failed to init bsdnet\n", stderr );

  Task_name[ 1 ] = rtems_build_name( 'O', 'V', 'E', 'N' );

  status = rtems_task_create(
    Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ]
  );

  status = rtems_task_start( Task_id[ 1 ], oven_task, 1 );

  status = rtems_message_queue_create(
    rtems_build_name( 'Q', '1', ' ', ' ' ),
    1,
    sizeof(uint8_t),
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id
  );


//#define AUTOMATIC_TEST

#ifdef AUTOMATIC_TEST  
	while (1) {
		uint16_t i;
		
		for(i=0; i<sizeof(testpattern);i++){
			timer_status = tick();
		
			if(timer_status)
				msg=evTimeout;
			else
				msg=testpattern[i];
    
    	status = rtems_message_queue_send( Queue_id, &msg, sizeof(uint8_t) );
    
			if ( status != RTEMS_SUCCESSFUL )
      	fputs( "send did not work\n", stderr );

    	status = rtems_task_wake_after( get_ticks_per_second()/10);
    }
    
    printf("Test finished\n");
    while(1){
			status = rtems_task_wake_after( get_ticks_per_second());
		}
  }

#else 

		init_keyboard();
		printf("Unknown key. Use + for inc; - for dec; o for open and c for close.\n");
		printf("             Use 'p' to set medium power; 'P' to set high power\n");		
		do{
			
			timer_status = tick();
			
			if(timer_status)
				msg=evTimeout;
			else if(kbhit()){
				char ch=readch();
				if(ch=='+')
					msg=evInc;
				else if(ch=='-')
					msg=evDec;
				else if(ch=='c')
					msg=evDoorClosed;
				else if(ch=='o')
					msg=evDoorOpen;
				else if(ch=='q'){
					close_keyboard();
					exit(0);					
				}else if(ch=='P'){
					pwr = PWR_HIGH;
					msg=evPwr;
				}else if(ch=='p'){
					pwr = PWR_LOW;
					msg=evPwr;
				}else{
//					printf("Unknown key %c. Use + for inc; - for dec; o for open and c for close.\n");
//					printf("             Use 'p' to set medium power; 'P' to set high power\n",ch);
					msg=OVEN_NO_MSG;
				}
			}else{
				msg=OVEN_NO_MSG;
			}

    	/* fprintf( stderr, "send 0x%d\n", count ); */
    	status = rtems_message_queue_send( Queue_id, &msg, sizeof(uint8_t) );
    
			if ( status != RTEMS_SUCCESSFUL )
      	fputs( "send did not work\n", stderr );

    	status = rtems_task_wake_after( get_ticks_per_second()/10);

		
		}while(1);

#endif


  status = rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             10
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    5
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 10

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
