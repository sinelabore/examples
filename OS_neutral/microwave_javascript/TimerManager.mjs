// Helper class that allows to create multipe timers for JS statemachines.
// The tick function must be called cyclically depending on the needed resolution.
// Here it is intended that the tick is called every 100ms.
export const timer_resolution = 100; // 
export const onesec=10; // tobe updated if timer_resolution changes

// TimerManager class definition
export function TimerManager() {
  this.timers = [];
  this.timerIdCounter = 0;

  // Indicates the timer state (stopped, running, or paused).
  this.timerstate = Object.freeze({
    stopped: 0,
    running: 1,
    paused: 2,
  });
  
  // Create a new timer with a timeout value in ms.
  // This function initializes a timer with the specified timeout, 
  // calculates the number of timer ticks, logs initial 
  // values, and returns a unique timer identifier.
  this.createTimer = function(timeout, myEvent){
    const timerId = ++this.timerIdCounter;  
    this.timers[timerId] = {
      state: this.timerstate.stopped,
      // Number of timer ticks calculated from `timeout` and `timer_resolution`.
      presetval: Math.floor(timeout / timer_resolution),
      // Current timer value, initialized same as `presetval`.
      actualval: Math.floor(timeout / timer_resolution),
      //Indicates whether the timer is cyclic (repeats) or not.
      cyclic: false,
      // Event to be fired if timeout happens
      evt: myEvent,
    }
    return timerId;
  }

  // Start a timer either cyclically or single-shot
  // If the the presetval is zero the timer is actually not started
  this.startTimer = function(timerId, isCyclic){
    const timer = this.timers[timerId];
    if (timer && timer.actualval == 0 && timer.presetval==0 && isCyclic==false){
      console.log("Try to start timer with timeout value==0");
    }else{
      console.log("started timer with tid=" + timerId);
      timer.state = this.timerstate.running;
      timer.cyclic = isCyclic;
      timer.actualval=timer.presetval;
    }
  }

  // Convinience function to create and start a timer at once
  this.createAndStartTimer = function(timeout, evt, isCyclic){
    const timerId = this.createTimer(timeout,evt);
    this.startTimer(timerId, isCyclic);
    return timerId;
  }

  // Stops a timer that was running or paused before
  // After it was stopped it can be started at any time again.
  this.stopTimer = function(timerId){
    const timer = this.timers[timerId];
    if (timer){
      timer.state = this.timerstate.stopped;
      timer.actualval = 0;
    }
  }

  // Pause a running timer. The actual value of the
  // actual timeout and preset is kept.
  this.pauseTimer = function(timerId){
    const timer = this.timers[timerId];
    if (timer && (timer.state == this.timerstate.running)){
      timer.state = this.timerstate.paused;
    }
  }

  // Continues a paused timer 
  this.continueTimer = function(timerId){
    const timer = this.timers[timerId];
    if (timer && (timer.state == this.timerstate.paused)){
      timer.state = this.timerstate.running;
    }
  }

  // Increment the timeout of a timer
  this.getPreset = function(timerId){
    const timer = this.timers[timerId];
    if (timer){
      return timer.actualval;
    }else{
      return 0;
    }
  }

  this.getPresetInSec = function(timerId){
    return Math.floor(this.getPreset(timerId)/10,onesec);
  }

  // Increment the timeout of a timer by 1s
  this.incTimer = function(timerId){
    const timer = this.timers[timerId];
    if (timer){
      timer.actualval+=onesec;
      timer.presetval=timer.actualval;
    }
  }

  // Decrement the timeout of a timer
  this.decTimer = function(timerId){
    const timer = this.timers[timerId];
    if (timer && timer.actualval > 0){
        timer.actualval-=onesec;
        timer.presetval=timer.actualval;
    }
  }

  // This function iterates over the existing running timers
  // and decrements the actualval time value of the timers.
  // If the timeout of a timer reachs 0 it is added to the
  // expired timers list. If a timer is cyclic it is reloaded.
  this.tick = function(){
    const dueTimers = [];

    // Get an array of timer IDs using Object.keys()
    const timerIds = Object.keys(this.timers);
    
    // Iterate over the timer IDs
    for (var i = 0; i < timerIds.length; i++) {
      var timerId = timerIds[i];
      var timer = this.timers[timerId];
      //console.log(timer);
      if (timer.state == this.timerstate.running) {
        timer.actualval--;
        if (timer.actualval === 0) {
          // timeout
          dueTimers.push(timer.evt);
          if (timer.cyclic) {
            // set timeout to preset
            timer.actualval = timer.presetval;
          } else {
            // Stop non-cyclic timers
            this.stopTimer(timerId);
          }
        }
      }
    }

    return dueTimers;
  };
}
