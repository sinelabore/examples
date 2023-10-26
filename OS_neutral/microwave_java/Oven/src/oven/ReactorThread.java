package oven;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import oven.Model.Reactor;


/**
 * This class runs the generated state machine in a thread. 
 * The thread is blocked until events are present in
 * the event input queue. The event is pulled out and the event
 * is sent to the state machine for processing. 
 * TODO: find a way to stop the thread in a good way
 * 
 * @author pmueller
 */
public class ReactorThread implements Runnable {
    private BlockingQueue<Reactor.Events> queue=null;
    private volatile boolean run;
    private Thread thread=null;
    Reactor reactor=null;
    CustomTimer tm;
    
    public ReactorThread(){
        queue = new ArrayBlockingQueue(30);
        reactor = new Reactor();
        tm = reactor.createTimerService(queue);
        thread = new Thread(this);
        run=true;
    }
    
    /**
     * To be called externally to start the threads
     */
    public void start(){
        thread.start();
        tm.startTimerThread();
        
    }
    
    public BlockingQueue<Reactor.Events> getMsgQueue(){
        return queue;
    }
    
    /**
     * Run the state machine and wait for new events 
     * to be processed by the machine.
     */
    @Override
    public void run(){      
        while(run){
            reactor.initialize();
            System.out.println("ReactorThread is created waiting for events");
            while (true) { 
                try{
                     Reactor.Events evt = queue.take(); 
                     //System.out.println("Got " + evt + "\n");
                     reactor.processEvent(evt);
                }catch(InterruptedException e) {
                    e.printStackTrace();
                }
            }            
        }
        
    }
}
