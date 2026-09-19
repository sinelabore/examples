/*
 * (c) 2026 Sinelabore Software Tools GmbH. All rights reserved.
 */
private import ScalarValues::*;
private import SI::*;

package LogicBasic{

	// What a wire carries: the level of one boolean signal.
	attribute def BoolSignal {
		attribute v : Boolean;
	}

	// An output drives a wire; an input is the conjugate of one (~BoolPort).
	port def BoolPort {
		attribute boolPortPayload : BoolSignal;
	}

    //
    // Signal 
    //

    // only emits true each run
	part def ConstTrue{

		out port q : BoolPort;

		action emitTrue{
			send new BoolSignal(true) via q;
		}

		state ConstTrueSM {
			do emitTrue;
			entry; then Running;
			state Running;
		}
	}

    // only emits true each run
	part def ConstFalse{

		out port q : BoolPort;

		action emitTrue{
			send new BoolSignal(false) via q;
		}

		state ConstFalseSM {
			do emitTrue;
			entry; then Running;
			state Running;
		}
	}
	

	// A free running source: q toggles every halfPeriod seconds. Nothing drives it,
	// so it is what gives a network of constant blocks something that changes.
	part def AsyncBlink {
		out port q : BoolPort;

		attribute ton : Real default 0.2; // on time
		attribute toff : Real default 0.2; // off time

		attribute qVal : Boolean default false;

		action emit{ send new BoolSignal(qVal) via q; }

		state BLINKSM {
			entry; then Low;
			do emit;

			state Low{
				entry assign qVal:=false;
			}
			accept after toff [SI::second] then High;

			state High{
				entry assign qVal:=true;
			}
			accept after ton [SI::second] then Low;
		}
	}

	// Single output pulse based on execution not time
	part def SingleClockPulse {
		out port q : BoolPort;

		attribute qVal : Boolean default false;

		action exec {
			first start;
			then assign qVal := not qVal;
			then send new BoolSignal(qVal) via q;
		}

		state PULSESM {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// single pulse block
	part def TonToffPulse {
		out port q : BoolPort;

		attribute qVal : Boolean default false;

		attribute ton : Real default 0.5; // on time
		attribute toff : Real default 0.2; // off time

		action emit { send new BoolSignal(qVal) via q; }

		state TonToffPulseSM {
			entry; then Low;
			do emit;

			state Low{
				entry assign qVal:=false;
			}
			accept after toff [SI::second] then High;

			state High{
				entry assign qVal:=true;
			}
			accept after ton [SI::second] then Done;

			state Done{
				entry assign qVal:=false;
			}
		}
	} 	   
    
	// One input copied to four outputs.
	//
	// A part's in port is a queue with a single reader, so it can stand for exactly one port
	// inside it: a net that has to reach four blocks cannot be a delegated in port, and a
	// clock is the usual case. An out port has as many receivers as it likes, so this block
	// is what turns the one into the other.
	//
	// Not a multiplexer, which picks one of several inputs, nor a demultiplexer, which routes
	// its input to one selected output: this copies its input to all four, every scan.
	part def BoolFanOut4 {
		in port i : ~BoolPort;
		out port o0 : BoolPort;
		out port o1 : BoolPort;
		out port o2 : BoolPort;
		out port o3 : BoolPort;

		attribute v : Boolean default false;

		action exec {
			action takeI accept s : BoolSignal via i {
				assign v := s.v;
			}
			then send new BoolSignal(v) via o0;
			then send new BoolSignal(v) via o1;
			then send new BoolSignal(v) via o2;
			then send new BoolSignal(v) via o3;
		}

		state FANSM {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// A D Flip-Flop: on a rising edge of clk, q takes whatever d is holding. Where a T_FF's
	// next output depends on its own present one - all it can do is invert - a D_FF's depends
	// only on its input, which is what makes a chain of them shift rather than count.
	part def D_FF {
		in port d : ~BoolPort;
		in port clk : ~BoolPort;
		out port q : BoolPort;

		attribute dVal : Boolean default false;
		attribute c : Boolean default false;
		attribute old : Boolean default false;
		attribute qVal : Boolean default false;

		action exec {
			action takeD accept sd : BoolSignal via d {
				assign dVal := sd.v;
			}
			then action takeClk accept sc : BoolSignal via clk {
				assign c := sc.v;
			}
			then if (old==false) and (c==true) {
				assign qVal := dVal;
			}
			then assign old := c;
			then send new BoolSignal(qVal) via q;
		}

		state DFFSM {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// A D_FF that captures on either edge of clk rather than only on the rising one, so a
	// clock that changes every scan shifts every scan. A rising edge needs the clock to go
	// low and high again, which costs two scans; there is no way to make a level change
	// twice in one scan, because a port queues what is sent to it and a block takes one
	// event per run - send both levels and the second waits for the next scan anyway.
	part def D_FF_AnyEdge {
		in port d : ~BoolPort;
		in port clk : ~BoolPort;
		out port q : BoolPort;

		attribute dVal : Boolean default false;
		attribute c : Boolean default false;
		attribute old : Boolean default false;
		attribute qVal : Boolean default false;

		action exec {
			action takeD accept sd : BoolSignal via d {
				assign dVal := sd.v;
			}
			then action takeClk accept sc : BoolSignal via clk {
				assign c := sc.v;
			}
			then if (old != c) {
				assign qVal := dVal;
			}
			then assign old := c;
			then send new BoolSignal(qVal) via q;
		}

		state DFFANYSM {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// A T_FF that toggles on either edge of clk rather than only on the rising one, so a
	// clock that changes every scan advances it every scan. The plain T_FF has to see its
	// clock fall again before it can count once more, which costs a second scan.
	part def T_FF_AnyEdge {
		in port clk : ~BoolPort;
		out port q : BoolPort;

		attribute c : Boolean default false;
		attribute old : Boolean default false;
		attribute qVal : Boolean default false;

		action exec {
			action takeClk accept s : BoolSignal via clk {
				assign c := s.v;
			}
			then if (old != c) {
				assign qVal := not qVal;
			}
			then assign old := c;
			then send new BoolSignal(qVal) via q;
		}

		state TFFANYSM {
			do exec;
			entry; then Running;
			state Running;
		}
	}

	// A T Flip-Flop (TFF) is a 1-bit memory element that
	// toggles its output when the payload = 1 and the arrives of new data
	part def T_FF {
		in port clk : ~BoolPort;
		out port q : BoolPort;

		attribute c : Boolean default false;
		attribute old : Boolean default false;
		attribute qVal : Boolean default false;

		action exec {
			action takeClk accept s : BoolSignal via clk {
				assign c := s.v;
			}
			then if (old==false) and (c==true) {
				assign qVal := not qVal;
			}
			then assign old := c;
			then send new BoolSignal(qVal) via q;
		}

		state TFFSM {
			do exec;
			entry; then Running;
			state Running;
		}
	}
}