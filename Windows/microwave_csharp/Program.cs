using System;

//
//  This example implements a microwave oven. The core logic (Oven.cs) was generated 
//  from an UML state machine diagram.
//  
//  The figure below shows the architecture of the program.
//  Interaction between the state machine and the other code is via a queue.
//  User can send simulated events via keyboard to the statemachine.
//  The timer is controlled from the state machine code. And also sends timeout
//  events to the queue.
//
//                         (start, stop ... timer)
//          +-----------------<----------------------------------------+
//          |                                                          |
//          |                                                          |
//+---------+---------+           +-----------------+          +-------+----------+
//|                   | evTimeout |                 |          |                  |
//|      Timer        +---------->+      Queue      +--------->+  State Machine   |
//|                   |           |                 |          |   (generated)    |
//+-------------------+           +-----------------+          |                  |
//                                   ^                         +------------------+
//                                   |    
//+-------------------+              |    
//|                   |  events      |  
//|     Key Input     +--------------+   
//|                   |                  
//+-------------------+                  
//                                    
class Program
{
  private static Oven? ovenStateMachine;
  private static Radiator? radiator;
  private static Queue<Oven.Events> q;
  private static Timer tm;

  static void Main(string[] args){

    // Setup the microwave oven and helper classes (timer, q, )
    ovenStateMachine = new Oven();
    radiator = new Radiator();
    q = new Queue<Oven.Events>();
    tm = new Timer(q,Oven.Events.evTimeout);

    ovenStateMachine.tm = tm;
    ovenStateMachine.q = q;
    ovenStateMachine.radiator=radiator;

    ovenStateMachine.Initialize();

    Console.WriteLine("Microwave oven simulation:");
    Console.WriteLine("+  increases cooking time");
    Console.WriteLine("-  decreases cooking time");    
    Console.WriteLine("O  opens the door");
    Console.WriteLine("C  closes the door");
    Console.WriteLine("L  sets radiator power to low");
    Console.WriteLine("H  sets radiator power to high");
    Console.WriteLine("Q  quit");
    Console.WriteLine("");
    Console.WriteLine("Usage hints:");    
    Console.WriteLine("- It is possible to inc/dec time while cooking");
    Console.WriteLine("- Once cooking is finished the door must be opened first");
    Console.WriteLine("- Door can be opened and closed while cooking");
    Console.WriteLine("- Example sequence: L++++++oc");



    while (true){
      if (Console.KeyAvailable) {
        var key = Console.ReadKey(true).Key;
        switch (key){
          case ConsoleKey.O:
              q.Enqueue(Oven.Events.evDoorOpen);
              break;
          case ConsoleKey.C:
              q.Enqueue(Oven.Events.evDoorClosed);
              break;
          case ConsoleKey.H:
              q.Enqueue(Oven.Events.evPwrHigh);
              break;
          case ConsoleKey.L:
              q.Enqueue(Oven.Events.evPwrLow);
              break;                    
          case ConsoleKey.Add:
            q.Enqueue(Oven.Events.evInc);
            break;
          case ConsoleKey.Subtract:
            q.Enqueue(Oven.Events.evDec);
            break;
          case ConsoleKey.Q:
              return;
        }
      }

      while (q.Count > 0) {
          //Console.WriteLine("process q");
          ovenStateMachine.ProcessEvent(q.Dequeue());
      }
      Thread.Sleep(10);
    }
  }
}

