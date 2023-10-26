package oven.Model;

import java.util.concurrent.BlockingQueue;
import oven.*;

/**
 * Base class of the generated state machine.
 * The base class creates the helper classes that
 * are used by the generated state machine code.
 * I.e. the Radiator, LED and Timer.
 * @author pmueller
 */
public class ReactorBase {
     
    protected CustomTimer tm=null; // timer service
    protected Radiator radiator=null;
    protected int id, idLed; // timer id
    protected Led led;
    
    public ReactorBase(){
        radiator = new Radiator();
        led = new Led();
    }
    
    public CustomTimer createTimerService(BlockingQueue<Reactor.Events> queue){
        tm = new CustomTimer(queue);

        return tm;
    }
}
