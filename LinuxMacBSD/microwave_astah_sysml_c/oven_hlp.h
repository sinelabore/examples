#ifndef __STARTER_HLP_H__
#define __STARTER_HLP_H__

// Defines for the timer
typedef enum {
	OFF=0,
	ON=1,
	PAUSE=2
} T_TIMER_STATE;

// Defines for Power Control
typedef enum{
	PWR_LOW=1,
	PWR_HIGH=2
} T_PWR;

// timer related functions

// starts timer
// Returns either ON if started or OFF if not started (e.g. preset=0)
T_TIMER_STATE timer_start(void);

// stops timer
void stop_timer(void);

// either returns ON or OFF or PAUSE
T_TIMER_STATE timer_status(void);

unsigned char tick();

// shortens timeout by 1s
void timer_dec(void);

// increments timeout by 1s
void timer_inc(void);

// returns initial timeout time
unsigned char timer_preset(void);

// pause timer
void timer_pause(void);

// resume timer
void timer_cont(void);

//
// other helpers
//

// Sets power 
void ovenSetPwr(T_PWR pwr);

// Switch off microwave generator
void ovenOff(void);

// Switch on microwave generator with preset
void ovenOn(void);

// Set preset value of timer to zero
void timer_clear(void);

#endif
