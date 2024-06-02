using System;
using System.Timers;

//
// Simple timer class that can realize cyclic or single shot timers
// If expired a predefined event is sent to the given message queue.
//
public class Timer
{
    private System.Timers.Timer timer;  // Specify the full namespace
    private State currentState;
    private uint countDownValue = 0;
    private uint delayTime = 0;
    private bool cyclic;
    private bool debug=false;
    
    private const uint TIMER_RESOLUTION = 1000; // 1s
    private const uint TICK_RESOLUTION = 100; //0.1s

    public Queue<Oven.Events> q;
    Oven.Events ev;

    enum State
    {
        Initial,
        Running,
        Stopped,
        Paused
    }

    public void IncrementTimer(){
        delayTime+=TIMER_RESOLUTION/TICK_RESOLUTION; 
        countDownValue+=TIMER_RESOLUTION/TICK_RESOLUTION; // allows inc when started
        Console.WriteLine("Timer timeout time = {0}",countDownValue);
    }

    public void DecrementTimer(){
        if(countDownValue>=TIMER_RESOLUTION/TICK_RESOLUTION){
            delayTime-=TIMER_RESOLUTION/TICK_RESOLUTION; 
            countDownValue-=TIMER_RESOLUTION/TICK_RESOLUTION; // allows dec when started
            Console.WriteLine("Timer timeout time = {0}",countDownValue);
        }else{
            //Console.WriteLine("Dec results in 0->elapse");
            countDownValue=0;
            TimerElapsed(null,null);
            StopTimer();
        }
    }

    public void PauseTimer(){
        if(currentState == State.Running) currentState = State.Paused;
        Console.WriteLine("Timer paused");
    }

    public void ContinueTimer(){
        if(currentState == State.Paused) currentState = State.Running;
        Console.WriteLine("Timer continued");
    }    

    public Timer(Queue<Oven.Events> queue, Oven.Events ev){

        this.q = queue;
        this.ev = ev;

        timer = new System.Timers.Timer();
        currentState = State.Initial;
        timer.Interval = TICK_RESOLUTION;
        timer.Elapsed += TimerElapsed;
        timer.AutoReset = true;
        timer.Enabled = true;
    }

    public uint PresetTimer(){
        return delayTime;
    }

    public void StartTimer(bool cyclic)
    {
        if((currentState == State.Stopped || currentState == State.Initial) && delayTime>0){
            this.cyclic = cyclic;
            countDownValue = delayTime;
            currentState = State.Running;
            Console.WriteLine(cyclic ? "Repeated timer started: {0}" : "Singel-shot timer started:  {0}",delayTime);
        }
    }

    public void StopTimer()
    {
        if(currentState == State.Running){
            currentState = State.Stopped;
            delayTime=0;
            countDownValue=0;
            Console.WriteLine("Timer stopped.");
        }
    }

    private void TimerElapsed(object? sender, ElapsedEventArgs? e)
    {
        if(currentState == State.Running){
            if(countDownValue>0) {
                countDownValue-=1;
            }else{
                // timeout!
                if(!cyclic){
                    // single shot
                    StopTimer();
                    q.Enqueue(ev);
                    Console.WriteLine("Timer expired and stopped");
                }else{
                    // reload
                    countDownValue = delayTime;
                    q.Enqueue(ev);
                    Console.WriteLine("Timer expired and reloaded");
                }
            }
        }
        //Console.Write(".");
    }
}
