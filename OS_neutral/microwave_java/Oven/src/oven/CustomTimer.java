package oven;


import java.util.ArrayList;
import java.util.concurrent.BlockingQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Simple timer class that provides timer services for the state machine.
 * A timer can be either cyclic or single shot. Upon timeout the event
 * specified at timer creation time will be put into the message queue.
 * Multiple timers can be created. Each timer is identified with 
 * an own timer id returned by the create method.
 * The id must be provided to the methods to identify the timer.
 * @author pmueller
 * @param <Q> queue to put events into
 * @param <E> Event that should be put into the queue upon timeout
 */
public class CustomTimer <Q,E> implements Runnable {
    private final BlockingQueue<E> eventQueue;
    private volatile boolean run=true;
    private Thread thread=null;
    private int timerId=0;
    private enum TimerStatus {STOP, RUN, PAUSE};
    public enum TimerType {SINGLE_SHOT, CYCLIC};
    private final ArrayList<TimerData> timers = new ArrayList<>();
    
    private class TimerData{
        public long interval=0;
        public long preset=0;
        public TimerStatus status=TimerStatus.STOP;
        public TimerType type=TimerType.SINGLE_SHOT;
        public E event;
    }


    
    public CustomTimer(BlockingQueue<E> eventQueue) {
        this.eventQueue = eventQueue;
    }
    
    @Override
    public void run(){
        System.out.println("Timer thread started");
        while(run){
            
            for(TimerData timer : timers){
                if(timer.status == TimerStatus.RUN){
                    timer.interval -= 100;
                    if(timer.interval <= 0){
                        if(timer.type == TimerType.SINGLE_SHOT){
                            // stop timer
                            timer.status =TimerStatus.STOP;
                        }else{
                            //reload timer
                            timer.interval=timer.preset;
                        }
                        //System.out.println("Timer callback executed!");
                        eventQueue.add(timer.event);                         
                    }
                }
            }
            
            try {
                Thread.sleep(100);
            } catch (InterruptedException ex) {
                Logger.getLogger(CustomTimer.class.getName()).log(Level.SEVERE, null, ex);
            }
      
        }
    }
    
    /**
     * Start timer thread
     *
     */
    public void startTimerThread(){
        thread = new Thread(this);
        thread.start();
    }

    /**
     * Creates a new timer
     * @param event to send to the message queue once the timer signals a timeout
     * @param type of timer - either single shot or cyclic
     * @param timeout time in ms when the timer signals a timeout
     * @return id representing the timer identification. Uniquely identifies a timer.
     */
    public int createTimer(E event, TimerType type, int timeout){
        TimerData timer = new TimerData();
        timer.interval = 0;
        timer.preset = timeout;
        timer.type = type;
        timer.event = event;
        int currentTimerId = timerId++;
        timers.add(currentTimerId, timer);
        return currentTimerId;
    }
    
    /**
     * Start timer
     * @param id
     */
    public void start(int id) {
        TimerData timer = timers.get(id);
        timer.status = TimerStatus.RUN;
        System.out.println("Timer " +id+ " Started");
    }

    /**
     * Stop timer and set delay to 0
     * @param id
     */
    public void stop(int id) {
        TimerData timer = timers.get(id);
        timer.status = TimerStatus.STOP;
        timer.interval=0;
        System.out.println("Timer " +id+ " Stopped");
    }
    
    /**
     * Return the current set delay
     * @param id
     * @return delay time in ms
     */
    public long preset(int id){
        TimerData timer = timers.get(id);
        return timer.interval;
    }
       
    /**
     * Pause timer
     * @param id
     */
    public void pause(int id){
        TimerData timer = timers.get(id);
        timer.status = TimerStatus.PAUSE;
        System.out.println("Timer " +id+ " Paused");      
    }

    /**
     * Continue paused timer
     * @param id
     */
    public void cont(int id){
        TimerData timer = timers.get(id);
        if(timer.status == TimerStatus.PAUSE){
            timer.status = TimerStatus.RUN;
            System.out.println("Continue " +id+ " Timer");
        }
    }
     
    /**
     * Increment delay by 1s
     * @param id
     */
    public void inc(int id){
        TimerData timer = timers.get(id);
        timer.interval+=1000;
        System.out.println("Timeout " +id+ " now "+ timer.interval);
    }
    
    /**
     * Increment delay by 1s
     * @param id
     */
    public void dec(int id){
        TimerData timer = timers.get(id);
        if(timer.interval>=1000){
            timer.interval-=1000;
        }       
        System.out.println("Timeout " +id+ " now "+ timer.interval);
    }
};