package oven;

import oven.Model.*;

/**
 * Simple main class to setup demonstrate the usage of the
 * generated state machine code.
 * A bunch of events are sent to the machine to trigger
 * reactions.
 * @author pmueller
 */
public class Main {
    
    /**
     * @param args the command line arguments
     * @throws java.lang.InterruptedException
     */
    public static void main(String[] args) throws InterruptedException {
              
        ReactorThread reactorThread = new ReactorThread();
        reactorThread.start();
        
        Thread.sleep(1000);
        
        var queue = reactorThread.getMsgQueue();

            
        queue.add(Reactor.Events.REACTOR_NO_MSG);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evInc);
        queue.add(Reactor.Events.evDoorClosed);
        queue.add(Reactor.Events.evDec);
        Thread.sleep(3000);
        queue.add(Reactor.Events.evDoorOpen);
        Thread.sleep(3000);
        queue.add(Reactor.Events.evDoorClosed);

            
        while(true)Thread.sleep(100);
    }    
}
